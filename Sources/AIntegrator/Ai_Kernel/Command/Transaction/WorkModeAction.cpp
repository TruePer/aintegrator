////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "WorkModeAction.h"

#include "Logger.h"
#include "Common/ICore.h"
#include "internal/Ai_Primitives.pb.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "State/AppState.h"



using namespace ai::command;

WorkModeAction::WorkModeAction(const QString& hostId, const QString& cardId)
    : Transaction(hostId)
    , _cardId(cardId)
{
}

void WorkModeAction::execute()
{
	auto cards = ai::protocol::ProtocolHelper::mutableCards(core()->state()->integratorOptions(), hostId().toStdString(), _cardId.toStdString());

	if(cards.isEmpty())
		return;

	auto* card = cards.at(0);

	if(!card)
		return;

	if(!card->has_state() || !card->state().has_simple_state() || !card->state().simple_state().has_mode_status()){
		ai::error() << QString("Can't send control command. simple_state don't have mode_status");
		return;
	}

	auto modeStatus = card->state().simple_state().mode_status();
	auto actionType = ai::proto::ControlAction::CA_START_WORK;

	switch (modeStatus) {
	case ai::proto::PM_IDLE:
		actionType = ai::proto::ControlAction::CA_START_WORK;
		break;

	case ai::proto::PM_RUN:
		actionType = ai::proto::ControlAction::CA_STOP_WORK;
		break;

	case ai::proto::PM_STARTING:
	case ai::proto::PM_STOPPING:
		ai::debug() << QString("Work mode is updating. Abort action.");
		return; // Ничего не делаем, выходим
	}

	kernel::AiMessage message;
	message.set_host_id(hostId().toStdString());
	message.mutable_proto()->set_type(proto::AiMessage::CONTROL_ACTION_REQUEST);
	message.mutable_proto()->set_id(id());

	auto* requset = message.mutable_proto()->mutable_control_action_request();
	requset->mutable_action()->set_type(actionType);
	requset->mutable_action()->set_card_id(_cardId.toStdString());

	sendMessage(message);
}

void WorkModeAction::processResponse(const ai::kernel::AiMessage& message)
{
	if(message.proto().type() != proto::AiMessage::CONTROL_ACTION_RESPONSE
	|| !message.proto().has_control_action_response()) {
		ai::error() << QObject::tr("Recieved incorrect response type: %1. Expected: CONTROL_ACTION_RESPONSE").arg(message.proto().type());
		return;
	}

	const auto& response = message.proto().control_action_response();

	if(response.report().code() != ai::proto::DONE) {
		ai::error() << QObject::tr("CONTROL_ACTION_RESPONSE error. Code: %1, Description: %2")
		                    .arg(response.report().code())
		                    .arg(QString::fromStdString(response.report().description()));
	}
}

void WorkModeAction::processTimeout()
{
	auto cards = ai::protocol::ProtocolHelper::mutableCards(core()->state()->integratorOptions(), hostId().toStdString(), _cardId.toStdString());

	for(auto* card : cards) {
		card->mutable_state()->Clear();
	}
}

