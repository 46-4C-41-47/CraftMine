#pragma once

#include <mutex>
#include <queue>
#include <thread>
#include <iostream>
#include <functional>


class ThreadPool
{
private:
	bool shutdown;

	std::mutex mutex;

	std::queue<std::function <void(void)>> jobs;
	std::vector<std::thread> threads;

	void threadWork(int threadIndex);

public:
	ThreadPool(int nbOfThreads);
	~ThreadPool();

	template<typename returnType>
	void submit(std::function<returnType (void)> func);

	void submitNoReturn(std::function<void (void)> func);

	void stop() { shutdown = true; }
};
