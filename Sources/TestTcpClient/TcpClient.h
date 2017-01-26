////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTimer>



class QTcpSocket;

namespace ai {
namespace proto {
class AiMessage;
}
}

class TcpClient : public QObject
{
	Q_OBJECT
public:
	explicit TcpClient(const QString& moduleId, QObject* parent = nullptr);

signals:
	void messageRecieved(const QString& message);

public:
	void sendHandShake();
	void sendParameters();
	void sendStatus();

private:
	void sendData(const QByteArray& data);
	void sendMessage(ai::proto::AiMessage& message);
	void processMessage(ai::proto::AiMessage& message);

private slots:
	void onReadData();
	void tryToConnect();
	void onConnected();
	void onDisconnected();

private:
	QTcpSocket*                _tcpSocket;
	quint32                    _expectedBlockSize;
	QString                    _moduleId;
	QTimer                     _timer;
	bool                       _isConnected;
};

#endif // TCPCLIENT_H
