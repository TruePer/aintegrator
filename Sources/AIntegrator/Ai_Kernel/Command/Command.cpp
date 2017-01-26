////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Command.h"
#include "Common/ICore.h"



using namespace ai::command;

Command::Command(const QString& hostId)
    : _hostId(hostId)
    , _core(nullptr)
{
}

void Command::processResponse(const ai::kernel::AiMessage& /*message*/)
{
	// Do nothink
}

void Command::processTimeout()
{
	// Do nothink
}

bool Command::isNeedResponse()
{
	return false;
}

bool Command::isTimedOut()
{
	return false;
}

quint32 Command::id() const
{
	return _id;
}

QString Command::hostId() const
{
	return _hostId;
}

ai::common::ICore* Command::core() const
{
	return _core;
}

void Command::execute(ai::common::ICore* core)
{
	static quint32 id = 0;

	_id = id++;
	_core = core;
	execute();
}
