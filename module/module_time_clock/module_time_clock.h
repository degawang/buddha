#pragma once

#include <deque>
#include <queue>
#include <mutex>
#include <chrono>
#include <future>
#include <vector>
#include <thread>
#include <atomic>
#include <numeric>
#include <iostream>
#include <functional>
#include <condition_variable>

#include <module_time_clock.h>

namespace module {
	class TimeClock {
	public:
		TimeClock() {
			__tic();
		}
		~TimeClock() = default;
	public:
		template<typename _object = std::chrono::microseconds>
		long long get_clock_count() {
			return std::chrono::duration_cast<_object>(__toc()).count();
		}
	private:
		void __tic() {
			__clock = std::chrono::high_resolution_clock::now();
		}
		std::chrono::nanoseconds __toc() {
			auto _return_count = std::chrono::high_resolution_clock::now() - __clock;
			__clock = std::chrono::high_resolution_clock::now();
			return _return_count;
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> __clock;
	};
}
