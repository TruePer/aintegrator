////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_COMMAND_HANDSHAKERESPONSE_H
#define AI_COMMAND_HANDSHAKERESPONSE_H

#include "Command/Command.h"



namespace ai {
namespace command {

/**
 * Ответ хосту на команду установления соединения
 */
class Kernel_EXPORT HandshakeResponse : public Command
{
public:
	HandshakeResponse(const kernel::AiMessage& response);

private:
	void execute();
};

} // namespace command
} // namespace ai

#endif // AI_COMMAND_HANDSHAKERESPONSE_H
