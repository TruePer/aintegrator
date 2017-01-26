////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef AI_COMMON_THREADSAVEQUEUE_H
#define AI_COMMON_THREADSAVEQUEUE_H

#include "Kernel_Export.h"

#include <QMutex>
#include <QQueue>
#include <QSharedPointer>



namespace ai {
namespace common {

/// Thread safe queue for store data chuncks
template<typename Data>
class ThreadSaveQueue
{
public:
	ThreadSaveQueue();

	typedef QSharedPointer<ThreadSaveQueue<Data>> SPtr;

public:
	void enqueue(const Data& dataChunck);
	bool checkEmptyAndDequeue(Data& dataChunck);
	bool isEmpty() const;
	quint32 size() const;

private:
	QQueue<Data>       _data;
	mutable QMutex     _mutex;

};

#include "ThreadSaveQueue.hpp"

}
}
#endif // AI_COMMON_THREADSAVEQUEUE_H
