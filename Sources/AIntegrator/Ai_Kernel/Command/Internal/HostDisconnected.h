////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_COMMAND_HOSTDISCONNECTED_H
#define AI_COMMAND_HOSTDISCONNECTED_H

#include "Command/Command.h"



namespace ai {
namespace command {

class Kernel_EXPORT HostDisconnected : public Command
{
public:
	HostDisconnected(const QString& hostId);

private:
	void execute();
};

} // namespace common
} // namespace ai

#endif // AI_COMMAND_HOSTDISCONNECTED_H
