////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_QUERY_QUERY_H
#define AI_QUERY_QUERY_H

#include "Command/ICommand.h"
#include "Common/Core.h"
#include "internal/Ai_Primitives.pb.h"

#include <QObject>
#include <QQueue>



namespace ai {
namespace query {

/**
 * Class for processing responses
 */
class Query : public QObject
{
	Q_OBJECT
public:
	explicit Query(ai::common::Core* core);

public:
	void addCommand(command::ICommand::SPtr command);

private slots:
	void onRecievedMessageFromHost(const ai::kernel::AiMessage& message);

private:
	void timerEvent(QTimerEvent* event);
	void processMessage(const ai::kernel::AiMessage& message);
	void processHandshakeRequest(const ai::kernel::AiMessage& request);
	void processSetParametersRequest(const ai::kernel::AiMessage& request);
	void processSetStateRequest(const ai::kernel::AiMessage& request);

private:
	common::Core*                            _core;
	QQueue<command::ICommand::SPtr>          _outCommandQueue;
	QHash<quint32, command::ICommand::SPtr>  _waitCommands;
	QQueue<ai::kernel::AiMessage>            _inMessageQueue;
};

} // namespace query
} // namespace ai

#endif // AI_QUERY_QUERY_H
