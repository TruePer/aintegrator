////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_COMMAND_HOSTCONNECTED_H
#define AI_COMMAND_HOSTCONNECTED_H

#include "Command/Command.h"



namespace ai {
namespace command {

class Kernel_EXPORT HostConnected : public Command
{
public:
	HostConnected(const QString& hostId);

private:
	void execute();
};

} // namespace command
} // namespace ai

#endif // AI_COMMAND_HOSTCONNECTED_H
