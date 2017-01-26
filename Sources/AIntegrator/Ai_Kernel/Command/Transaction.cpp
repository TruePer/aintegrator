////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Logger.h"
#include "Transaction.h"
#include "internal/Ai_Primitives.pb.h"
#include "Common/ICore.h"
#include "Transport/TcpServer.h"

#include <QTime>



using namespace ai::command;

Transaction::Transaction(const QString& hostId)
    : Command(hostId)
    , _needResponse(true)
    , _timeoutMsec(2000)
    , _passedTime(new QTime())
    , _id(0)
{
}

Transaction::~Transaction()
{
	delete _passedTime;
}

bool Transaction::isNeedResponse()
{
	return _needResponse;
}

bool Transaction::isTimedOut()
{
	return _passedTime->elapsed() > _timeoutMsec;
}


void Transaction::setNeedResponse(bool need)
{
	_needResponse = need;
}

void Transaction::setTimeout(quint32 timeoutMsec)
{
	_timeoutMsec = timeoutMsec;
}

void Transaction::sendMessage(ai::kernel::AiMessage& message)
{
	if(hostId().isEmpty()) {
		ai::warn() << QObject::tr("Empty host id");
	}

	message.set_host_id(hostId().toStdString());

	static quint32 counter = 0;

	if(!message.proto().has_id()) {
		message.mutable_proto()->set_id(counter);
	}

	core()->server()->sendMessage(message);
	_passedTime->start();
}
