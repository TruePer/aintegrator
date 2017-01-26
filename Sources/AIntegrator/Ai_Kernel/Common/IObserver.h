////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AI_OBSERVER_H
#define AI_OBSERVER_H

#include "Command/ICommand.h"
#include "Kernel_Export.h"



namespace ai {

namespace state {
class AppState;
}

namespace common {

class Kernel_EXPORT IObserver
{
public:
	virtual void update(const ai::state::AppState* state) = 0; // Переопределяется как слот
	virtual void sendCommand(const ai::command::ICommand::SPtr command) = 0; // Переопределяется как сигнал
	virtual void initialize() {}
};

}
}

#endif // AI_OBSERVER_H
