////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_LAUNCHER_PROCESSLAUNCHER_H
#define AI_LAUNCHER_PROCESSLAUNCHER_H

#include "internal/Ai_ExecutionModule.pb.h"



namespace ai {
namespace launcher {

/**
 * Реализация запуска процессов из Интегратора
 */
class ProcessLauncher
{
public:
	ProcessLauncher();

public:
	static bool startProcess(const ai::kernel::ExecutionModule::LaunchParameters& launchParameters);
};

} // namespace launcher
} // namespace ai

#endif // AI_LAUNCHER_PROCESSLAUNCHER_H
