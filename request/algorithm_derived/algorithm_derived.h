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

#include "algorithm_base.h"
#include "pattern_singleton_pattern.h"

namespace algorithm {
	class DrivedAlgorithm : public BaseAlgorithm, public pattern::SingletonPattern<DrivedAlgorithm> {
	public:
		virtual void hello() const override {
			std::cout << "hello DrivedAlgorithm !" << std::endl;
		}
	};
}
