////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AI_COMMAND_SETPOSTPARAMETERS_H
#define AI_COMMAND_SETPOSTPARAMETERS_H

#include "Command/Transaction.h"
#include "host/Ai_PostParameters.pb.h"



namespace ai {
namespace command {

class Kernel_EXPORT SetPostParameters : public Transaction
{
public:
	SetPostParameters(const QString& executionModuleId, const proto::PostParameters& parameters);

private:
	void execute();
	void processResponse(const kernel::AiMessage& message);
	void processTimeout();

private:
	proto::PostParameters _parameters;
};

} // namespace commoand
} // namespace ai

#endif // AI_COMMAND_SETPOSTPARAMETERS_H
