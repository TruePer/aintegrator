////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AI_COMMON_CORE_H
#define AI_COMMON_CORE_H

#include "ICore.h"

#include <QJSValue>
#include <QObject>
#include <QTimer>


class QTranslator;

namespace ai {

namespace state {
class AppState;
}

namespace transport {
class TcpServer;
}

namespace query {
class Query;
}

namespace common {

/**
 * Ядро приложения. Содержит состояние приложения, TCP-сервер, обработчик входящих запросов и проч.
 * По мимо прочего реализован механизм Обозревателя - обзекты, унаследованные от IObserver могут
 * регистрироваться и получать обновления состояния, а так же отправлять команды на обработку
 */
class Kernel_EXPORT Core : public QObject, public ICore
{
	Q_OBJECT
	Q_PROPERTY(ai::state::AppState* state READ state NOTIFY appStateChanged)

signals:
	void appStateChanged(const ai::state::AppState* appState);

public:
	Core(QObject* parent = 0);
	~Core();

	static void registerTypes();

public:
	Q_INVOKABLE void initialize();
	Q_INVOKABLE void registerObserver(const QJSValue& observer);
	state::AppState* state() const;
	transport::TcpServer* server() const;

public slots:
	void addCommand(ai::command::ICommand::SPtr command);

private:
	void registerObserver(QObject* observer);

private slots:
	void timerEvent(QTimerEvent* event);
	void onHostConnected(const QString& hostId);
	void onHostDisconnected(const QString& hostId);
	void onTryToListenPort();

private:
	state::AppState*                _appState;
	transport::TcpServer*           _tcpServer;
	query::Query*                   _query;
	QTranslator*                    _ruTranslator;
	QTimer                          _restartListenTimer;
};

}
}

#endif // AI_COMMON_CORE_H
