#include "../include/ThreadPool.h"


ThreadPool::ThreadPool(int nbOfThreads) : shutdown(false)
{
	threads.reserve(nbOfThreads);

	for (int i = 0; i < nbOfThreads; i++)
		threads.emplace_back(std::thread(&ThreadPool::threadWork, this, i));
}


ThreadPool::~ThreadPool()
{
	shutdown = true;

	for (int i = 0; i < threads.size(); i++)
		threads[i].join();
}


void ThreadPool::threadWork(int threadIndex)
{
	bool retrievedFunc = false;
	std::function<void(void)> func;

	while (!shutdown)
	{
		mutex.lock();
		if (!jobs.empty())
		{
			func = jobs.front();
			jobs.pop();
			retrievedFunc = true;
		}
		mutex.unlock();

		if (retrievedFunc)
			func();

		retrievedFunc = false;
	}
}

void ThreadPool::submitNoReturn(std::function<void (void)> func)
{
	mutex.lock();

	jobs.push(func);

	mutex.unlock();
}
