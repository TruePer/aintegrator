////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ICORE_H
#define ICORE_H

#include "Command/ICommand.h"
#include "Common/IObserver.h"



namespace ai {

namespace state {
class AppState;
}

namespace transport {
class TcpServer;
}

namespace common {

class Kernel_EXPORT ICore
{
public:
	virtual void initialize() = 0;

public:
	virtual state::AppState* state() const = 0;

	virtual void addCommand(command::ICommand::SPtr command) = 0;
//	virtual void addLog(const QString& shortMessage, const QString& detailedMessage = QString(), log::MessageType::Enum type = log::MessageType::INFO, const QString& sender = QString()) = 0;
	virtual transport::TcpServer* server() const = 0;
	virtual void registerObserver(QObject* observer) = 0;
};

}
}

#endif // ICORE_H
