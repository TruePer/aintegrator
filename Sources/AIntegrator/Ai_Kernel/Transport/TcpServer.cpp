////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Logger.h"
#include "TcpServer.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "internal/Ai_Primitives.pb.h"

#include <QDataStream>
#include <QTcpServer>
#include <QTcpSocket>



using namespace ai::transport;

TcpServer::TcpServer(QObject* parent)
    : QObject(parent)
    , _tcpServer(new QTcpServer(this))
    , _expectedBlockSize(0)
{
	connect( _tcpServer, SIGNAL(newConnection()), SLOT(onNewConnection()) );
}

bool TcpServer::listen(quint16 port)
{
	if (_tcpServer->listen(QHostAddress::Any, port)) {
		ai::info() << tr("TCP server started. Listen to port %1").arg(port);
		return true;
	}

	ai::error() << tr("ERROR! TCP server was not started. Port %1 is busy. Trying to restart after 5 seconds.").arg(port);
	return false;
}

/**
 * Отправляет сообщение клиенту с идентификатором hostId
 * @param hostId
 * @param message
 */
void TcpServer::sendMessage(const ai::kernel::AiMessage& message)
{
	if(!message.IsInitialized()) {
		ai::error() << tr("Message initialization error: %1").arg(message.InitializationErrorString().c_str());
		return;
	}

	auto hostId = QString::fromStdString(message.host_id());
	if(!_tcpClients.contains(hostId)) {
		ai::error() << tr("Can not send data. Unknown client with host id: %1").arg(hostId);
		return;
	}

	auto data = ai::protocol::ProtocolHelper::serializeMessage(message.proto());
	quint32 size = data.size();
	auto sendData = QByteArray((const char *)&size, sizeof(size));
	sendData.append(data);
#ifndef QT_DEBUG
	ai::logProtobuf() << message.DebugString().c_str();
#endif

	auto* clientSocket = _tcpClients[hostId];

	clientSocket->write(sendData);
}

/**
 * Вызывается, когда было установлено соединение с клиентом
 */
void TcpServer::onNewConnection()
{
	auto* clientSocket = _tcpServer->nextPendingConnection();
	int clientDecriptor = clientSocket->socketDescriptor();

	ai::log() << tr("New TCP connection from %1").arg(clientSocket->peerAddress().toString());

	connect(clientSocket, SIGNAL(readyRead()), SLOT(onReadClientData()));
	connect(clientSocket, SIGNAL(disconnected()), SLOT(onCloseConnection()));
}

/**
 * Вызывается, когда было закрыто соединение с клиентом
 */
void TcpServer::onCloseConnection()
{
	auto* clientSocket = dynamic_cast<QTcpSocket*>(sender());

	if(!clientSocket)
		return;

	auto hostId = clientSocket->property("name").toString();

	ai::log() << tr("Host '%1' disconnected").arg(hostId);

	_tcpClients.remove(hostId);
	clientSocket->deleteLater();

	emit hostDisconnected(hostId);
}

/**
 * Читает полученные данные из сокета
 */
void TcpServer::onReadClientData()
{
	QTcpSocket* clientSocket = dynamic_cast<QTcpSocket*>(sender());

	if(!clientSocket)
		return;

	while(true) {
		if(!_expectedBlockSize) {
			if(clientSocket->bytesAvailable() < sizeof(_expectedBlockSize)) {
				break; // Данных нет, или их недостаточно даже для определения размера блока
			}

			// Читаем размер блока
			QDataStream in(clientSocket);
			in.setByteOrder(QDataStream::LittleEndian);
			in >> _expectedBlockSize;
		}

		// Проверим, все ли данные пришли от клиента
		if(clientSocket->bytesAvailable() < _expectedBlockSize) {
			break; // Ждем следущую порцию данных.
		}

		// Читаем данные из сокета
		QScopedArrayPointer<char> data(new char[_expectedBlockSize]);
		int countRead = clientSocket->read(data.data(), _expectedBlockSize);

		// Проверяем, все ли считалось (да, это параноя, но все же...)
		if(countRead != _expectedBlockSize) {
			ai::error() << tr("Expected data size in not equal to readed data size");
			break;
		}

		// Десериализуем полученное сообщение
		ai::kernel::AiMessage message;
		ai::protocol::ProtocolHelper::deserializeMessage(message.mutable_proto(), data.data(), _expectedBlockSize);

		if(message.proto().type() == ai::proto::AiMessage_Type_HANDSHAKE_REQUEST
		&& message.proto().has_handshake_request()) {
			auto hostId = QString::fromStdString(message.proto().handshake_request().host_id());
			clientSocket->setProperty("name", hostId);
			_tcpClients[hostId] = clientSocket;

			ai::log() << tr("Post '%1' connected").arg(hostId);

			emit hostConnected(hostId);
		}

		auto hostId = clientSocket->property("name").toString();
		if(!_tcpClients.contains(hostId)) {
			ai::warn() << tr("Can not process recieved data. Unknown client with host id: %1").arg(hostId);
		}

		message.set_host_id(hostId.toStdString());

		_expectedBlockSize = 0;
#ifndef QT_DEBUG
		ai::logProtobuf() << message.DebugString().c_str();
#endif

		emit recievedMessage(message);
	}
}
