////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SetPostParameters.h"


#include "Logger.h"
#include "Common/ICore.h"
#include "internal/Ai_Primitives.pb.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "State/AppState.h"

namespace ai {
namespace command {

SetPostParameters::SetPostParameters(const QString& executionModuleId, const proto::PostParameters& parameters)
    : Transaction(executionModuleId)
    , _parameters(parameters)
{
}

void SetPostParameters::execute()
{
	kernel::AiMessage message;
	message.set_host_id(hostId().toStdString());
	message.mutable_proto()->set_type(proto::AiMessage::SET_PARAMETERS_REQUEST);
	message.mutable_proto()->set_id(id());

	auto* requset = message.mutable_proto()->mutable_set_parameters_request();
	requset->mutable_parameters()->CopyFrom(_parameters);

	ai::log() << _parameters.DebugString().c_str();

	sendMessage(message);
}

void SetPostParameters::processResponse(const kernel::AiMessage& message)
{
	if(message.proto().type() != proto::AiMessage::SET_PARAMETERS_RESPONSE
	|| !message.proto().has_set_parameters_response()) {
		ai::error() << QObject::tr("Recieved incorrect response type: %1. Expected: SET_PARAMETERS_RESPONSE").arg(message.proto().type());
		return;
	}

	const auto& response = message.proto().set_parameters_response();

	if(response.report().code() == proto::DONE)
		return; // Все хорошо

	if(response.has_parameters()) {
		// TODO: Поменять параметры
	}
	else {
		// TODO: Оставить те переметры, которые были до
	}
}

void SetPostParameters::processTimeout()
{

}

} // namespace commoand
} // namespace ai

