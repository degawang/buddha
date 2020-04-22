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

#include "pattern_singleton_pattern.h"

namespace algorithm {
	class BaseAlgorithm {
	public:
		BaseAlgorithm() = default;
		~BaseAlgorithm() = default;
	public:
		virtual void hello() const {
			std::cout << "hello BaseAlgorithm !" << std::endl;
		}
	};
}
