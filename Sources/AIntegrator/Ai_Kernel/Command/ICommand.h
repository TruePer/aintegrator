////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ICOMMAND_H
#define ICOMMAND_H

#include "Kernel_Export.h"

#include <QDataStream>
#include <QSharedPointer>



namespace ai {

namespace kernel {
class AiMessage;
}

namespace common {
class ICore;
}

namespace command {

/**
 * Базовый интерфейсный класс для реализации команд Интегратора
 */
class Kernel_EXPORT ICommand
{
public:
	typedef QSharedPointer<ICommand> SPtr;

public:
	virtual ~ICommand(){}

public:
	virtual void execute(common::ICore* core) = 0;
	virtual void processResponse(const kernel::AiMessage& message) = 0;
	virtual void processTimeout() = 0;

	virtual bool isNeedResponse() = 0;
	virtual bool isTimedOut() = 0;
	virtual quint32 id() const = 0;
};

}
}

#endif // ICOMMAND_H
