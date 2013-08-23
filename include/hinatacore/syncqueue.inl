#include <hinatacore/common.h>

HINATA_NAMESPACE_BEGIN

template <typename T>
SyncQueue<T>::SyncQueue()
	: done(false)
{

}

template <typename T>
void SyncQueue<T>::Enqueue( const T& data )
{
	std::unique_lock<std::mutex> lock(mutex);
	queue.push(data);
	notEmpty.notify_one();
}

template <typename T>
void SyncQueue<T>::Dequeue( T& data )
{
	std::unique_lock<std::mutex> lock(mutex);

	notEmpty.wait(lock, [this]{ return !queue.empty() || done; });
	if (done)
	{
		return;
	}

	data = std::move(queue.front());
	queue.pop();
}

template <typename T>
void SyncQueue<T>::SetDone()
{
	std::unique_lock<std::mutex> lock(mutex);
	done = true;
	notEmpty.notify_all();
}

HINATA_NAMESPACE_END