#ifndef __HINATA_CORE_SYNC_QUEUE_H__
#define __HINATA_CORE_SYNC_QUEUE_H__

#include <hinatacore/common.h>
#include <queue>
#include <mutex>
#include <condition_variable>

HINATA_NAMESPACE_BEGIN

/*!
	Synchronized queue.
	\tparam T Internal value type.
*/
template <typename T>
class SyncQueue
{
public:

	SyncQueue();

private:

	SyncQueue(const SyncQueue&);
	SyncQueue(SyncQueue&&);
	void operator=(const SyncQueue&);
	void operator=(SyncQueue&&);

public:

	void Enqueue(const T& data);
	void Dequeue(T& data);
	bool Done() { return done; }
	void SetDone();

private:

	bool done;
	std::queue<T> queue;
	std::mutex mutex;
	std::condition_variable notEmpty;

};

HINATA_NAMESPACE_END

#include "syncqueue.inl"

#endif // __HINATA_CORE_SYNC_QUEUE_H__