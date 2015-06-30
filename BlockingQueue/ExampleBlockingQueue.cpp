// BlockingQueue.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BlockingQueue.cpp"


int _tmain(int argc, _TCHAR* argv[])
{
	BlockingQueue<int> bq(20);
	bq.put(2);

	return 0;
}

