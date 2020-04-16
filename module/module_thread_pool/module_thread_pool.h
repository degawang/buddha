#pragma once

#include <deque>
#include <queue>
#include <mutex>
#include <future>
#include <vector>
#include <thread>
#include <atomic>
#include <numeric>
#include <iostream>
#include <functional>
#include <condition_variable>

#include "pattern_singleton_pattern.h"

namespace module{

	class ThreadPool : public pattern::SingletonPattern<ThreadPool> {
	public:
		~ThreadPool() {
			__stop_task();
		}
    private:
        explicit ThreadPool(int thread_num) : __stop(false) {
			__thread_num = (thread_num > 0 ? thread_num : std::thread::hardware_concurrency());
            for (size_t i = 0; i < __thread_num; ++i) {
				__threads.push_back(std::thread([this] {
					while (!__stop.load(std::memory_order_acquire)) {
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(this->__mutex);
							__cond.wait(lock, [this] {return !this->__tasks.empty() || __stop.load(std::memory_order_acquire); });
	                        if (__stop.load(std::memory_order_acquire))
								return;
							task = std::move(__tasks.front());
							__tasks.pop();
						}
						task();
					}
				}));
			}
        }
	public:
        template <typename _func, typename... _args>
		auto commit_task(_func&& func, _args&&... args) -> std::future<typename std::result_of<_func(_args...)>::type> {
			typedef typename std::result_of<_func(_args...)>::type return_type;
			auto t = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<_func>(func), std::forward<_args>(args)...));
			auto ret = t->get_future();
			{
				std::unique_lock<std::mutex> lock(__mutex);
				__tasks.emplace([t] {
					(*t)();
				}); 
			}
			__cond.notify_one();
			return ret;
		}
	private:
        void __stop_task() {
			__stop.store(true, std::memory_order_release);
			__cond.notify_all();
            for(auto &ref : __threads){
                if(ref.joinable()){
                    ref.join();
                }
            }
		}
    private:
        int __thread_num;
        std::mutex __mutex;
		std::atomic<bool> __stop;
        std::condition_variable __cond;
        std::vector<std::thread> __threads;
		std::queue<std::function<void()>> __tasks;
	private:
		friend pattern::SingletonPattern<ThreadPool>;
    };
}
