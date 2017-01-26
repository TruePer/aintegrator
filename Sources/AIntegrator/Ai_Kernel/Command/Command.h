////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_COMMAND_COMMAND_H
#define AI_COMMAND_COMMAND_H

#include "Command/ICommand.h"



namespace ai {
namespace command {

/**
 * Базовый класс команд, не требующих ответа.
 * Через него реализуются внутенние команды - без отправки сообщений клиентам,
 * и обработчики сообщений от клиентов (пока обработка реализована в объекте Query)
 */
class Kernel_EXPORT Command : public ICommand
{
public:
	Command(const QString& hostId = QString());

protected:
	virtual void execute() = 0;
	void processResponse(const kernel::AiMessage& message);
	void processTimeout();

	bool isNeedResponse();
	bool isTimedOut();
	quint32 id() const;

protected:
	QString hostId() const;
	common::ICore* core() const;

private:
	void execute(common::ICore* core);

private:
	QString        _hostId;
	common::ICore* _core;
	quint32        _id;
};

} // namespace command
} // namespace ai

#endif // AI_COMMAND_COMMAND_H
