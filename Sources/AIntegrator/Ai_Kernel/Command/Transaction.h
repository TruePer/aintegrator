////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_COMMAND_TRANSACTION_H
#define AI_COMMAND_TRANSACTION_H

#include "Command/Command.h"



namespace ai {
namespace command {

/**
 * Базовая команда для отправки сообщений клиенту с ожиданием ответа от него.
 * Содержит таймаут ответа, по истечении которого ожидание прекращается
 */
class Kernel_EXPORT Transaction : public Command
{
public:
	Transaction(const QString& hostId = QString());
	~Transaction();

protected:
	void execute() = 0;
	void processResponse(const kernel::AiMessage& message) = 0;
	void processTimeout() = 0;

	bool isNeedResponse();
	bool isTimedOut();

protected:
	void setNeedResponse(bool need);
	void setTimeout(quint32 timeoutMsec);
	void sendMessage(ai::kernel::AiMessage& message);

private:
	bool    _needResponse;
	quint32 _timeoutMsec;
	QTime*  _passedTime;
	quint32 _id;
};

} // namespace command
} // namespace ai

#endif // AI_COMMAND_TRANSACTION_H
