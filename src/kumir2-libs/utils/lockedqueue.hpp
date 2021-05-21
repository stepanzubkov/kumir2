#ifndef UTILS_LOCKEDQUEUE_HPP
#define UTILS_LOCKEDQUEUE_HPP
#include <queue>
#include <QSemaphore>
#include <QMutex>

namespace kumir2
{

/**

=== Interface:

	class LockedQueue<typename T> {
	public:
		bool empty() const;             // Returns true on empty, non blocking
		T dequeue();                    // Returns first item or blocks until filled
		void enqueue(const T & item);   // Puts an item into queue
		void reset(const T & nullitem); // Reset all waiters returning them provided value
		void clear();                   // Clears queue
	}


=== Usage Example:

	// Default used implemenations should be included before LockedQueue
	#include <QSemaphore>
	#include <QMutex>

	// Inlcude LockedQueue
	#include <utils/lockedqueue.hpp>

	void thread_sender_run(kumir2::LockedQueue & queue)
	{
		queue.enqueue(1);
		queue.enqueue(2);
		queue.enqueue(3);
	}

	void thread_consumer_run(kumir2::LockedQueue & queue)
	{
		int a = queue.dequeue();  // a = 1
		int b = queue.dequeue();  // b = 2;
		int c = queue.dequeue();  // c = 3;
		int d = queue.dequeue();  // Waits until
								  // queue not filled at least
								  // one element
	}

	void thread_watchdog_stop(kumir2::LockedQueue & queue)
	{
		queue.reset(4); // If some thread is waiting, unblock it by returning 4
	}

*/



template <typename T, class Semaphore = QSemaphore, class Mutex = QMutex>
class LockedQueue
{
public:
	bool empty() const
	{
		mutex_.lock();
		bool result = buffer_.empty();
		mutex_.unlock();
		return result;
	}

	T dequeue()
	{
		T result = nullitem_;
		semaphore_.acquire();
		mutex_.lock();
		if (!buffer_.empty()) {
			result = buffer_.front();
			buffer_.pop();
		}
		mutex_.unlock();
		return result;
	}

	void enqueue(const T &item)
	{
		mutex_.lock();
		buffer_.push(item);
		mutex_.unlock();
		semaphore_.release();
	}

	void reset(const T &nullitem)
	{
		mutex_.lock();
		nullitem_ = nullitem;
		mutex_.unlock();
		while (! semaphore_.available()) {
			semaphore_.release();
		}
	}

	void clear()
	{
		mutex_.lock();
		while (! buffer_.empty()) {
			buffer_.pop();
		}
		mutex_.unlock();
	}

private:
	std::queue<T> buffer_;
	Semaphore semaphore_;
	mutable Mutex mutex_;
	T nullitem_;
};
}

#endif
