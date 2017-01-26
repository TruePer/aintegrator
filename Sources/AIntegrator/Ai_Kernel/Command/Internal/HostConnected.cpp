////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "HostConnected.h"

#include "Common/ICore.h"
#include "internal/Ai_Primitives.pb.h"
#include "Protocol/Helper/ProtocolHelper.h"
#include "State/AppState.h"



using namespace ai::command;

HostConnected::HostConnected(const QString& hostId)
    : Command(hostId)
{
}

void HostConnected::execute()
{
	auto ems = ai::protocol::ProtocolHelper::mutableExecutionModules(core()->state()->integratorOptions(), hostId().toStdString());

	if(ems.isEmpty())
		return;

	for(auto* em : ems) {
		em->set_is_connected(true);
	}
}
