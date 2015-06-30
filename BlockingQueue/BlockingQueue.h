#pragma once

#include "stdafx.h"
#include "BlockingQueue.cpp"
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <iostream>

/*
 *	This class implements a item list that can be accessed  
 *	by multiple threads concurrently.
 */

template <class T> class BlockingQueue
{
private:
	std::queue<T> q;			/*Object's queue*/
	int max;					/*Max number of Items in the queue*/
	std::mutex m;				/*Mutex for thread-safe*/
	std::condition_variable cv;	/*Condition variable for inter-thread communication*/
public:
	BlockingQueue(int m);		/*Constructor*/
	~BlockingQueue();			/*Distructor*/
	void put(T t);				/*Function that inserts an element in the queue.*/
	bool get(T& t);				/*Function that gat an element by the queue.*/
	void close();				/*Function to close the queue*/
};

