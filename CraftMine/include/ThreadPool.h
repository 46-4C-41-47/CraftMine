#pragma once

#include <mutex>
#include <queue>
#include <future>
#include <thread>
#include <iostream>
#include <functional>


class ThreadPool
{
private:
	static const int THREAD_COUNT = 8;
	static ThreadPool* instance;
	bool shutdown;

	std::mutex mutex;

	std::queue<std::function <void(void)>> jobs;
	std::vector<std::thread> threads;

	void threadWork(int threadIndex);

	ThreadPool();

public:
	static ThreadPool* getInstance();
	~ThreadPool();

	template<typename returnType>
	std::future<returnType> submit(std::function<returnType (void)> func);

	void submitNoReturn(std::function<void (void)> func);

	void stop() { shutdown = true; }
};
