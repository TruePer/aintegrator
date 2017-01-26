////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "AppState.h"
#include "internal/Ai_Integrator.pb.h"

#include <QDir>


using namespace ai::state;

AppState::AppState()
    : _integratorOptions(new kernel::IntegratorOptions())
    , _logStorage(new ai::logger::LogStorage())
{
}

AppState::~AppState()
{
	delete _integratorOptions;
}

QString AppState::settingsDirectory()
{
	// Проверим наличие директории для хранения настроек 3gpp_gui
	const QString settingsDirName = QDir::homePath() + "/.AIntegrator";

	QDir settingsDir(settingsDirName);
	if(!settingsDir.exists()) {
		settingsDir.mkpath(settingsDirName);
	}

	return settingsDirName;
}

void AppState::setUnchanged()
{
}

const ConnectionSettings::ChRef& AppState::connectionSettings() const
{
	return _connectionSettings;
}

ConnectionSettings::ChRef& AppState::connectionSettings()
{
	return _connectionSettings;
}

void AppState::setConnectionSettings(const ConnectionSettings& connectionSettings)
{
	_connectionSettings.setRef(connectionSettings);
}

const ai::kernel::IntegratorOptions* AppState::integratorOptions() const
{
	return _integratorOptions;
}

ai::kernel::IntegratorOptions* AppState::integratorOptions()
{
	return _integratorOptions;
}

const ai::logger::LogStorage& AppState::logStorage() const
{
	return *_logStorage;
}

ai::logger::LogStorage& AppState::logStorage()
{
	return *_logStorage;
}

QString AppState::applicationTitle() const
{
	if(_integratorOptions->has_name()) {
		return QString::fromStdString(_integratorOptions->name());
	}

	return tr("A-Integrator");
}
