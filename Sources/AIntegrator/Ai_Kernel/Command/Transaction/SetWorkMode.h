////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_COMMAND_SETWORKMODE_H
#define AI_COMMAND_SETWORKMODE_H

#include "Command/Transaction.h"



namespace ai {
namespace command {

class Kernel_EXPORT SetWorkMode : public Transaction
{
public:
	SetWorkMode(const QString& executionModuleId, const QString& cardId, const QString& workModeId);

private:
	void execute();
	void processResponse(const kernel::AiMessage& message);
	void processTimeout();

private:
	QString _cardId;
	QString _workModeId;

};

} // namespace command
} // namespace ai

#endif // AI_COMMAND_SETWORKMODE_H
