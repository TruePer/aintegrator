////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Logger.h"
#include "ProcessLauncher.h"

#include <QProcess>



using namespace ai::launcher;

ProcessLauncher::ProcessLauncher()
{
}

bool ProcessLauncher::startProcess(const ai::kernel::ExecutionModule::LaunchParameters& launchParameters)
{
	auto program     = QString::fromStdString(launchParameters.path_to_executable());
	auto parameters  = QString::fromStdString(launchParameters.command_options());
	auto workingDir  = QString::fromStdString(launchParameters.work_directory());

	ai::log() << QObject::tr("Starting: %1\n   params: %2\n   work dir: %3").arg(program).arg(parameters) .arg(workingDir);

	parameters = parameters.simplified();
	QStringList parameterList;
	parameterList << parameters.split(' ');

	bool res = QProcess::startDetached(program, parameterList, workingDir);

	if(res) {
		ai::debug() << QObject::tr("DONE");
	}
	else {
		ai::warn() << QObject::tr("FAIL");
	}

	return res;
}
