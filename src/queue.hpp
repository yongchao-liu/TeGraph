#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class Queue {
	const size_t capacity;
	std::queue<T> queue;
	std::mutex mtx;
	std::condition_variable cond_full;
	std::condition_variable cond_empty;
	std::condition_variable cond_finish;

public:
	Queue(const size_t capacity) : capacity(capacity) {}
	inline void push(const T & item) {
		std::unique_lock<std::mutex> lck(mtx);
		cond_full.wait(lck, [&](){ return !is_full(); });
		queue.push(item);
		lck.unlock();
		cond_empty.notify_one();
	}

	inline T pop() {
		std::unique_lock<std::mutex> lck(mtx);
		cond_empty.wait(lck, [&](){ return !is_empty(); });
		auto item = queue.front();
		queue.pop();
		lck.unlock();
		cond_full.notify_one();
		if (is_empty()) cond_finish.notify_one();
		return item;
	}

	inline void wait_empty(){
		std::unique_lock<std::mutex> lck(mtx);
		cond_finish.wait(lck, [&]{ return is_empty(); });
		lck.unlock();
	}

	inline bool is_full(){
		return queue.size() == capacity;
	}

	inline bool is_empty(){
		return queue.empty();
	}

	inline size_t size(){
		return queue.size();
	}
};

#endif
