////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_COMMAND_LAUNCHEXECUTIONMODULE_H
#define AI_COMMAND_LAUNCHEXECUTIONMODULE_H

#include "Command/Command.h"



namespace ai {
namespace command {

/**
 * Команда запускает модуль
 */
class Kernel_EXPORT LaunchExecutionModule : public Command
{
public:
	LaunchExecutionModule(const QString& executionModuleId);

protected:
	void execute();

private:
	QString _executionModuleId;
};

} // namespace command
} // namespace ai

#endif // AI_COMMAND_LAUNCHEXECUTIONMODULE_H
