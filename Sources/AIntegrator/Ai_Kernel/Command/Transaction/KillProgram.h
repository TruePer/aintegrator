////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_COMMAND_KILLPROGRAM_H
#define AI_COMMAND_KILLPROGRAM_H

#include "Command/Transaction.h"



namespace ai {
namespace command {

class Kernel_EXPORT KillProgram : public Transaction
{
public:
	KillProgram(const QString& executionModuleId, const QString& cardId);

private:
	void execute();
	void processResponse(const kernel::AiMessage& message);
	void processTimeout();

private:
	QString _cardId;
};

} // namespace command
} // namespace ai

#endif // AI_COMMAND_KILLPROGRAM_H
