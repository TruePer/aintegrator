////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_COMMON_THREADSAVEQUEUE_H
# error "You shouldn't include this file directly!"
#endif
#include "ThreadSaveQueue.h"

#include <QMutexLocker>



using namespace ai::common;

template<typename Data>
ThreadSaveQueue<Data>::ThreadSaveQueue()
{
}

/**
 * Puts chunck to queue
 * @param chunck
 */
template<typename Data>
void ThreadSaveQueue<Data>::enqueue(const Data& dataChunck)
{
	QMutexLocker locker(&_mutex);
	_data.enqueue(dataChunck);
}

/**
 * Checks if queue is not empty and copy chunk to the reference
 * @param cunck - reference to result
 * @return false if queue is empty
 */
template<typename Data>
bool ThreadSaveQueue<Data>::checkEmptyAndDequeue(Data& dataChunck)
{
	QMutexLocker locker(&_mutex);

	if(_data.isEmpty())
		return false;

	dataChunck = _data.dequeue();

	return true;
}

/**
 * Checks if queue is empty
 * @return
 */
template<typename Data>
bool ThreadSaveQueue<Data>::isEmpty() const
{
	QMutexLocker locker(&_mutex);
	return _data.isEmpty();
}

/**
 * Returns size of queue
 * @return
 */
template<typename Data>
quint32 ThreadSaveQueue<Data>::size() const
{
	QMutexLocker locker(&_mutex);
	return _data.size();
}
