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

#include <pattern_singleton_pattern.h>

namespace module{

	// 线程池析构函数有问题
	class ThreadPool : public pattern::SingletonPattern<ThreadPool> {
    public:
        explicit ThreadPool(int expect_thread_num){
            thread_num = std::thread::hardware_concurrency();
            for (int i = 0; i < thread_num; i++) {
				threads.emplace_back(std::thread([this] {
					while (run) {
						std::unique_lock<std::mutex> lock(mutex);
						cond.wait(lock, [this] {return !tasks.empty() && run; });
						while (!tasks.empty()){
							auto task = std::move(tasks.front());
							task();
							tasks.pop();
						}
					}
				}));
			}
        }
   		~ThreadPool(){
			stop();
		}

        template <typename _func, typename... _args>
		std::future<typename std::result_of<_func(_args...)>::type> add(_func&& func, _args&&... args) {
			typedef typename std::result_of<_func(_args...)>::type return_type;
			if (run) {
				std::unique_lock<std::mutex> lock(mutex);
				auto t = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<_func>(func), std::forward<_args>(args)...));
				auto ret = t->get_future();
				tasks.emplace([t] {
					(*t)();
				});
				cond.notify_one();
				return std::move(ret);
			}
		}

		template <typename _func, typename... _args>
		auto __add(_func&& func, _args&&... args) -> std::future<decltype(func(args...))> {
			using return_type = decltype(func(args...));
			if (run) {
				std::unique_lock<std::mutex> lock(mutex);
				auto t = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<_func>(func), std::forward<_args>(args)...));
				auto ret = t->get_future();
				tasks.emplace([t] {
					(*t)();
				});
				cond.notify_one();
				return std::move(ret);
			}
		}

        void stop() {
            // 需要策略避免是空队列
            // range-for 的复制策略
			cond.notify_all();
            for(auto &ref : threads){
                if(ref.joinable()){
                    ref.join();
                }
            }
			//cond.notify_all();
			//auto it = threads.begin();
			//while (threads.end() != it) {
			//	if (it->joinable())
			//		it->join();
			//}
			run = false;
		}
    private:
        int thread_num;
        std::mutex mutex;
		std::atomic<bool> run;
        std::condition_variable cond;
        std::vector<std::thread> threads;
		std::queue<std::function<void()>> tasks;
	private:
		friend pattern::SingletonPattern<ThreadPool>;
    };

	// need to be reviewed
	class threadpool {
		using Task = std::function<void()>;

	public:
		threadpool(size_t size = 4)
			: _stop(false) {
			size = size < 1 ? 1 : size;

			for (size_t i = 0; i < size; ++i) {
				_pool.emplace_back(&threadpool::schedual, this);
			}
		}

		~threadpool() {
			shutdown();
		}

		// 提交一个任务
		template<class F, class... Args>
		auto commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
			using ResType = decltype(f(args...));// 函数f的返回值类型

			auto task = std::make_shared<std::packaged_task<ResType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
			{
				// 添加任务到队列
				std::lock_guard<std::mutex> lock(_taskMutex);

				_tasks.emplace([task]() {
					(*task)();
				});
			}

			_taskCV.notify_all(); //唤醒线程执行

			std::future<ResType> future = task->get_future();
			return future;
		}

	private:
		// 获取一个待执行的task
		Task get_one_task() {
			std::unique_lock<std::mutex> lock(_taskMutex);

			_taskCV.wait(lock, [this]() { return !_tasks.empty() || _stop.load(); }); // wait直到有task

			if (_stop.load()) {
				return nullptr;
			}

			Task task{ std::move(_tasks.front()) }; // 取一个task
			_tasks.pop();
			return task;
		}

		// 任务调度
		void schedual() {
			while (!_stop.load()) {
				if (Task task = get_one_task()) {
					task();
				}
			}
		}

		// 关闭线程池，并等待结束
		void shutdown() {
			this->_stop.store(true);
			_taskCV.notify_all();

			for (std::thread &thrd : _pool) {
				thrd.join();// 等待任务结束， 前提：线程一定会执行完
			}
		}

	private:
		// 线程池
		std::vector<std::thread> _pool;

		// 任务队列
		std::queue<Task> _tasks;

		// 同步
		std::mutex _taskMutex;
		std::condition_variable _taskCV;

		// 是否关闭提交
		std::atomic<bool> _stop;
	};

}
