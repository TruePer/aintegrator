////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Core.h"
#include "Command/Internal/HostConnected.h"
#include "Command/Internal/HostDisconnected.h"
#include "internal/Ai_Integrator.pb.h"
#include "Query/Query.h"
#include "Setting/IntegratorSettingsLoader.h"
#include "State/AppState.h"
#include "Transport/TcpServer.h"

#include <QtQml>



using namespace ai::common;
using namespace ai::transport;

Core::Core(QObject* parent)
    : QObject(parent)
    , _appState(new state::AppState())
    , _query(new query::Query(this))
    , _tcpServer(new TcpServer(this))
    , _ruTranslator(new QTranslator(this))
{
	_ruTranslator->load("AIntegrator_ru", ":/Translation/");
	startTimer(1000);

	connect(_tcpServer, SIGNAL(hostConnected(QString)), SLOT(onHostConnected(QString)));
	connect(_tcpServer, SIGNAL(hostDisconnected(QString)), SLOT(onHostDisconnected(QString)));
	connect(&_restartListenTimer, SIGNAL(timeout()), SLOT(onTryToListenPort()) );
}

Core::~Core()
{
	delete _appState;
}

/**
 * Регистрирует типы для подключения в QML
 */
void Core::registerTypes()
{
	qmlRegisterType<ai::common::Core>("ai.common", 1, 0,"Core");
	qmlRegisterType<ai::state::AppState>("ai.state", 1, 0,"AppState");
}

/**
 * Инициализация приложения
 */
void Core::initialize()
{
	// Читаем настройки из конфигурационного файла
	setting::IntegratorSettingsLoader::load(*_appState->integratorOptions());

	onTryToListenPort();
}

/**
 * Состояние приложения
 * @return
 */
ai::state::AppState* Core::state() const
{
	return _appState;
}

/**
 * Добавляет команду на обработку
 * @param command
 */
void Core::addCommand(ai::command::ICommand::SPtr command)
{
	_query->addCommand(command);
}

ai::transport::TcpServer* Core::server() const
{
	return _tcpServer;
}

void Core::registerObserver(QObject* observer)
{
	if(!observer) {
		qCritical("Observer is null!");
		return;
	}

	connect(this, SIGNAL(appStateChanged(const ai::state::AppState*)), observer, SLOT(update(const ai::state::AppState*)));
	connect(observer, SIGNAL(sendCommand(const ai::command::ICommand::SPtr)), SLOT(addCommand(ai::command::ICommand::SPtr)));
}

void Core::timerEvent(QTimerEvent* /*event*/)
{
	emit appStateChanged(_appState);
}

void Core::onHostConnected(const QString& hostId)
{
	addCommand(ai::command::ICommand::SPtr(new ai::command::HostConnected(hostId)));
}

void Core::onHostDisconnected(const QString& hostId)
{
	addCommand(ai::command::ICommand::SPtr(new ai::command::HostDisconnected(hostId)));
}

void Core::onTryToListenPort()
{
	// Запускаем сервер на прослушку
	const auto& connection = _appState->integratorOptions()->connection();

	if(connection.type() == kernel::ConnectionOptions::CT_TCP
	&& connection.has_tcp()) {
		if(!_tcpServer->listen(connection.tcp().port())) {
			_restartListenTimer.start(5000);
		}
		else {
			_restartListenTimer.stop();
		}
	}
}

void Core::registerObserver(const QJSValue& observer)
{
	if(observer.isObject()) {
		registerObserver(observer.toQObject());
	}
}
