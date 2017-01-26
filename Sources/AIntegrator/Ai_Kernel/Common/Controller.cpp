////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Controller.h"
#include "ICore.h"
#include "Logger.h"

#include "Command/Internal/LaunchExecutionModule.h"
#include "Command/Transaction/KillProgram.h"
#include "Command/Transaction/RaiseWindow.h"
#include "Command/Transaction/SetPostParameters.h"
#include "Command/Transaction/SetWorkMode.h"
#include "Command/Transaction/WorkModeAction.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "State/AppState.h"

#include <QApplication>
#include <QTranslator>



using namespace ai;
using namespace ai::common;

Controller::Controller(ICore* core, QObject* parent)
    : QObject(parent)
    , _core(core)
{
}

/**
 * Запускает все модули АРМа
 * @param workStationName
 */
void Controller::launchWorkStation(const QString& workStationName)
{
	const auto emList = ai::protocol::ProtocolHelper::executionModules(*_core->state()->integratorOptions(), workStationName);

	ai::logCommand() << tr("Try to launch work station %1").arg(workStationName);

	for(const auto& em : emList) {
		if(!em.is_connected()) {
			launchExecutionModule(QString::fromStdString(em.id()), QString());
		}
	}
}

/**
 * Посылает команду на остановку всем модулям, входящим в АРМ
 * @param workStationName
 */
void Controller::stopWorkStation(const QString& workStationName)
{
	const auto emList = ai::protocol::ProtocolHelper::executionModules(*_core->state()->integratorOptions(), workStationName);

	ai::log() << tr("Try to stop work station %1").arg(workStationName);

	for(const auto& em : emList) {
		if(em.is_connected()) {
			auto emId = QString::fromStdString(em.id());
			ai::log() << tr("Try to send kill command to module %1").arg(emId);
			_core->addCommand(command::Command::SPtr(new command::KillProgram(emId, QString())));
		}
	}
}

/**
 * Запустить модуль
 * @param executionModuleId
 */
void Controller::launchExecutionModule(const QString& executionModuleId, const QString& cardId)
{
	// Проверим, если модуль уже имеет статус (т.е. запущен), то отправим ему команду
	const auto& ems = ai::protocol::ProtocolHelper::mutableExecutionModules(_core->state()->integratorOptions(), executionModuleId.toStdString());

	if(ems.isEmpty()) {
		ai::error() << tr("Can't launch execution module %1. There no information").arg(executionModuleId);
		return;
	}

	ai::kernel::ExecutionModule* em = ems.at(0);

	if(em && em->is_connected()) {
		ai::log() << tr("Try to send CA_RAISE_WINDOW command to module %1").arg(executionModuleId);
		_core->addCommand(command::Command::SPtr(new command::RaiseWindow(executionModuleId, cardId)));
	}
	else {
		ai::log() << tr("Try to launch module %1").arg(executionModuleId);
		_core->addCommand(command::Command::SPtr(new command::LaunchExecutionModule(executionModuleId)));
	}
}

/**
 * Установить соответствующий режим работы
 * @param executionModuleId
 * @param cardId
 * @param workModeId
 */
void ai::common::Controller::setWorkMode(const QString& executionModuleId, const QString& cardId, const QString& workModeId)
{
	ai::log() << tr("Try to set work mode. Module: %1, Card: %2, WorkMode: %3")
	                    .arg(executionModuleId)
	                    .arg(cardId)
	                    .arg(workModeId);

	_core->addCommand(command::Command::SPtr(new command::SetWorkMode(executionModuleId, cardId, workModeId)));
}

/**
 * Выполнить текущее действие с режимом работы
 * @param executionModuleId
 * @param cardId
 */
void Controller::workModeAction(const QString& executionModuleId, const QString& cardId)
{
	ai::log() << tr("Try to current work mode action. Module: %1, Card: %2")
	                    .arg(executionModuleId)
	                    .arg(cardId);

	_core->addCommand(command::Command::SPtr(new command::WorkModeAction(executionModuleId, cardId)));
}

void Controller::setPostParameters(const QString& executionModuleId, const proto::PostParameters& parameters)
{
	ai::log() << tr("Try to set work station parameters. Module: %1")
	                    .arg(executionModuleId);

	_core->addCommand(command::Command::SPtr(new command::SetPostParameters(executionModuleId, parameters)));
}

void Controller::uiSelectLanguage(quint32 languageId)
{
	QString lang;

	if(languageId == 0) {
		lang = "ru";
	} else if(languageId == 2) {
		lang = "en";
	}
	auto* translator = new QTranslator(this);
	translator->load("AIntegrator_" +  lang, ":/Translation/");
	qApp->installTranslator(translator);
}

