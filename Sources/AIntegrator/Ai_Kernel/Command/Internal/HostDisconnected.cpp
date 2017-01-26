////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "HostDisconnected.h"
#include "Common/ICore.h"
#include "internal/Ai_Primitives.pb.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "State/AppState.h"



using namespace ai::command;

HostDisconnected::HostDisconnected(const QString& hostId)
    : Command(hostId)
{
}

void HostDisconnected::execute()
{
	QList<ai::kernel::ExecutionModule*> ems = ai::protocol::ProtocolHelper::mutableExecutionModules(core()->state()->integratorOptions(), hostId().toStdString());

	if(ems.isEmpty())
		return;

	for(auto* em : ems) {
		em->set_is_connected(false);

		for(int i = 0, size = em->cards_size(); i < size; ++i) {
			auto* card = em->mutable_cards(i);
			card->mutable_state()->Clear();
		}

		em->mutable_parameters()->Clear();
	}
}
