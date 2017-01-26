////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_COMMAND_RAISEWINDOW_H
#define AI_COMMAND_RAISEWINDOW_H

#include "Command/Transaction.h"



namespace ai {
namespace command {

class Kernel_EXPORT RaiseWindow : public Transaction
{
public:
	RaiseWindow(const QString& executionModuleId, const QString& cardId);

private:
	void execute();
	void processResponse(const kernel::AiMessage& message);
	void processTimeout();

private:
	QString _cardId;
};

} // namespace command
} // namespace ai

#endif // AI_COMMAND_RAISEWINDOW_H
