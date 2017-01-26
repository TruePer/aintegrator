////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SIGNALSENDER_H
#define SIGNALSENDER_H

#include "Command/ICommand.h"



namespace Common {

template<typename T>
class Kernel_EXPORT SignalSender
{
private:
	SignalSender();

public:
	static void registerSender(T sender)
	{
		instance() = sender;
	}

	static void sendCommand(ICommand::SPtr cmd)
	{
		instance()->sendCommand(cmd);
	}

private:
	static T& instance()
	{
		static T instance;
		return instance;
	}
};
}

#endif // SIGNALSENDER_H
