////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_COMMON_CONTROLLER_H
#define AI_COMMON_CONTROLLER_H

#include <QObject>
#include "Kernel_Export.h"



namespace ai {

namespace proto {
class PostParameters;
}

namespace common {

class ICore;

/**
 * Класс, реализующий контроллер. Объект класса регистрируестя в QML, затем вызываются методы, создающие команды для обработки в ядре
 */
class Kernel_EXPORT Controller : public QObject
{
	Q_OBJECT
public:
	explicit Controller(ICore* core, QObject* parent = 0);

public:
	Q_INVOKABLE void launchWorkStation(const QString& workStationName);
	Q_INVOKABLE void stopWorkStation(const QString& workStationName);
	Q_INVOKABLE void launchExecutionModule(const QString& executionModuleId, const QString& cardId);
	Q_INVOKABLE void setWorkMode(const QString& executionModuleId, const QString& cardId, const QString& workModeId);
	Q_INVOKABLE void workModeAction(const QString& executionModuleId, const QString& cardId);
	Q_INVOKABLE void setPostParameters(const QString& executionModuleId, const proto::PostParameters& parameters);

	// UI
	Q_INVOKABLE void uiSelectLanguage(quint32 languageId);

private:
	ICore* _core;
};

} // namespace common
} // namespace ai

#endif // AI_COMMON_CONTROLLER_H
