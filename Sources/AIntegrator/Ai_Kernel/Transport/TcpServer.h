////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_TRANSPORT_TCPSERVER_H
#define AI_TRANSPORT_TCPSERVER_H

#include <QMap>
#include <QObject>



class QTcpServer;
class QTcpSocket;

namespace ai {

namespace kernel {
class AiMessage;
}

namespace transport {

/**
 * Сервер входящих соединений. Хранит данные о клиентах, обрабатывет соединения, получает и отправляет сериализованные данные
 */
class TcpServer : public QObject
{
	Q_OBJECT
signals:
	void recievedMessage(const ai::kernel::AiMessage& message);
	void hostConnected(const QString& hostId);
	void hostDisconnected(const QString& hostId);

public:
	TcpServer(QObject* parent = nullptr);

	bool listen(quint16 port);
	void sendMessage(const ai::kernel::AiMessage& message);

private slots:
	void onNewConnection();
	void onCloseConnection();
	void onReadClientData();

private:
	QTcpServer*                _tcpServer;
	QMap<QString, QTcpSocket*> _tcpClients;
	quint32                    _expectedBlockSize;
};

} // namespace transport
} // namespace ai

#endif // AI_TRANSPORT_TCPSERVER_H
