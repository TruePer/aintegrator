////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "LaunchExecutionModule.h"
#include "Logger.h"
#include "Common/ICore.h"
#include "Launcher/ProcessLauncher.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "State/AppState.h"



using namespace ai::command;

LaunchExecutionModule::LaunchExecutionModule(const QString& executionModuleId)
    : _executionModuleId(executionModuleId)
{
}

void LaunchExecutionModule::execute()
{
	auto ems = ai::protocol::ProtocolHelper::executionModules(*core()->state()->integratorOptions());
	auto id = _executionModuleId.toStdString();

	for(const auto& em : ems ) {
		if(em.id() != id)
			continue;

		if(!em.launch_paramerers_size()) {
			ai::warn() << QObject::tr("Launch parameters for module '%1' were not found. Skip").arg(_executionModuleId);
			return;
		}

		ai::log() << QObject::tr("Module '%1' found. Starting...").arg(_executionModuleId);

		for(int i = 0, size = em.launch_paramerers_size(); i < size; ++i) {
			ai::launcher::ProcessLauncher::startProcess(em.launch_paramerers(i));
		}

		break;
	}
}
