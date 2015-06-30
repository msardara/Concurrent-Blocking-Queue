#include "stdafx.h"
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <iostream>


template <class T> class BlockingQueue
{
private:
	std::queue<T> q;			/*Object's queue*/
	int max;					/*Max number of Items in the queue*/
	std::mutex m;				/*Mutex for thread-safe*/
	std::condition_variable cv;	/*Condition variable for inter-thread communication*/
	bool closed;
public:
	BlockingQueue(int m);		/*Constructor*/
	~BlockingQueue();			/*Distructor*/
	void put(T t);				/*Function that inserts an element in the queue.*/
	bool get(T& t);				/*Function that gat an element by the queue.*/
	void close();				/*Function to close the queue*/
};

template<class T> BlockingQueue<T>::BlockingQueue(int m) : max(m)
{
}


template<class T> BlockingQueue<T>::~BlockingQueue()
{
}

template<class T> void BlockingQueue<T>::put(T t)
{
	std::unique_lock<std::mutex> ul(m);

	if (closed) throw std::runtime_error("The queue has been closed!");

	while (q.size() >= static_cast<unsigned int>(max))

		/* Reached the max number of items in the queue. We have to wait. The While loop is needed due to the presence of multiple threads.
		 * If a thread get an object and another thread puts into the queue (but that thread is not the current thread), we will stay in
		 * the loop and do not insert an object in the full queue.
		 */

		cv.wait(ul);			/*With this statement it is possible wait here without consume resources*/


	q.push(t);
	cv.notify_all();	/*It is necessary to notify all waiting-getting threads of the new item in the queue.*/

}

template <class T> bool BlockingQueue<T>::get(T& t)
{
	std::unique_lock<std::mutex> ul(m);
		
	/*The queue is empty but not closed. This means some process can put an item into queue in any moment.*/
	while (!q.size() && !closed)	/*While there is no element in the queue...*/
			cv.wait(ul);				/*This wait will be waked up by the notify_all() in some put.*/

	if (closed && !q.size())		/*Queue closed and empty. No thread can insert an element in the queue. Solution: return false!*/
		return false;

	t = q.front();					/*The front operation return the element but do not remove it. We need to call pop too.*/
	q.pop();
	cv.notify_all();				/*We will wake up all waiting-putting threads.*/

	return true;
}

template <class T> void BlockingQueue<T>::close()
{
	std::unique_lock<std::mutex> ul(m);
	closed = true;

	cv.notify_all(); /* We have to notify all threads that the queue has been closed. If there are threads waiting for put
					  * an item into the queue, these will put the item, because they have arrived when the queue was still open.
					  * The getting-threads will get the items while the queue will be empty. After this moment, no thread will
					  * put and no thread will get.
					  */

}