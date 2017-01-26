////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef APPSTATE_H
#define APPSTATE_H

#include "Log/LogStorage.h"
#include "State/ConnectionSettings.h"

#include <QObject>



namespace ai {

namespace kernel {
class IntegratorOptions;
}

namespace state {

/**
 * Класс, описывающий состояние приложения. Хранит данные о текущих параметрах системы
 * Состояние не может быть изменено из "верхних" модулей. Оно изменяется только в Core и Query
 */
class Kernel_EXPORT AppState : public QObject
{
	Q_OBJECT
	Q_PROPERTY(ai::logger::LogStorage* logStorage MEMBER _logStorage NOTIFY changed)

signals:
	void changed();

public:
	AppState();
	~AppState();

public:
	static QString settingsDirectory();

public:
	void setUnchanged();

	// Настройки подключения
	const ConnectionSettings::ChRef& connectionSettings() const;
	ConnectionSettings::ChRef& connectionSettings();
	void setConnectionSettings(const ConnectionSettings& connectionSettings);

	// Настройки Интегратора
	const kernel::IntegratorOptions* integratorOptions() const;
	kernel::IntegratorOptions* integratorOptions();

	// Контейнер логов для вывода на консоль
	const logger::LogStorage& logStorage() const;
	logger::LogStorage& logStorage();

	Q_INVOKABLE QString applicationTitle() const;

private:
	ConnectionSettings::ChRef     _connectionSettings;
	kernel::IntegratorOptions*    _integratorOptions;
	logger::LogStorage*           _logStorage;
};

}
}

#endif // APPSTATE_H
