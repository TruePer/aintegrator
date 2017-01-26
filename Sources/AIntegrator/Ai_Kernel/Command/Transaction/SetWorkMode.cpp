////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "SetWorkMode.h"

#include "Logger.h"
#include "Common/ICore.h"
#include "internal/Ai_Primitives.pb.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "State/AppState.h"



using namespace ai::command;

SetWorkMode::SetWorkMode(const QString& executionModuleId, const QString& cardId, const QString& workModeId)
    : Transaction(executionModuleId)
    , _cardId(cardId)
    , _workModeId(workModeId)
{
}

void SetWorkMode::execute()
{
	kernel::AiMessage message;
	message.set_host_id(hostId().toStdString());
	message.mutable_proto()->set_type(proto::AiMessage::SET_WORK_MODE_REQUEST);
	message.mutable_proto()->set_id(id());

	auto* request = message.mutable_proto()->mutable_set_work_mode_request();
	request->set_card_id(_cardId.toStdString());
	request->set_work_mode_id(_workModeId.toStdString());

	sendMessage(message);
}

void SetWorkMode::processResponse(const ai::kernel::AiMessage& message)
{
	if(message.proto().type() != proto::AiMessage::SET_WORK_MODE_RESPONSE
	|| !message.proto().has_set_work_mode_response()) {
		ai::error() << QObject::tr("Recieved incorrect response type: %1. Expected: SET_WORK_MODE_RESPONSE").arg(message.proto().type());
		return;
	}

	auto cards = ai::protocol::ProtocolHelper::mutableCards(core()->state()->integratorOptions(), hostId().toStdString(), _cardId.toStdString());

	for(auto* card : cards) {
		if(!card)
			return;

		const auto& response = message.proto().set_work_mode_response();

		if(card->type() == ai::proto::ST_SIMPLE) {
			card->mutable_state()->mutable_simple_state()->set_current_mode_id(response.work_mode_id());
		}
	}

	// TODO: Переключаться на предыдущий режим, если в ответе ошибка
}

void SetWorkMode::processTimeout()
{
	auto cards = ai::protocol::ProtocolHelper::mutableCards(core()->state()->integratorOptions(), hostId().toStdString(), _cardId.toStdString());
	for(auto* card : cards) {
		card->mutable_state()->Clear();
	}
}
