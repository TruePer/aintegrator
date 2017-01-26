////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "RaiseWindow.h"

#include "Logger.h"
#include "Common/ICore.h"
#include "internal/Ai_Primitives.pb.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "State/AppState.h"



using namespace ai::command;

RaiseWindow::RaiseWindow(const QString& executionModuleId, const QString& cardId)
    : Transaction(executionModuleId)
    , _cardId(cardId)
{
}

void RaiseWindow::execute()
{
	kernel::AiMessage message;
	message.set_host_id(hostId().toStdString());
	message.mutable_proto()->set_type(proto::AiMessage::CONTROL_ACTION_REQUEST);
	message.mutable_proto()->set_id(id());

	auto* requset = message.mutable_proto()->mutable_control_action_request();
	requset->mutable_action()->set_type(ai::proto::ControlAction::CA_RAISE_WINDOW);
	requset->mutable_action()->set_card_id(_cardId.toStdString());

	sendMessage(message);
}

void RaiseWindow::processResponse(const ai::kernel::AiMessage& message)
{
	if(message.proto().type() != proto::AiMessage::CONTROL_ACTION_RESPONSE
	|| !message.proto().has_control_action_response()) {
		ai::error() << QObject::tr("Recieved incorrect response type: %1. Expected: CONTROL_ACTION_RESPONSE").arg(message.proto().type());
		return;
	}
}

void RaiseWindow::processTimeout()
{
	auto cards = ai::protocol::ProtocolHelper::mutableCards(core()->state()->integratorOptions(), hostId().toStdString(), _cardId.toStdString());

	for(auto* card : cards) {
		if(card) {
			card->mutable_state()->Clear();
		}
	}
}
