#pragma once

#include <map>
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

#include "../../base/base.h"
#include "algorithm_base.h"
#include "pattern_singleton_pattern.h"

namespace module {
	class Reflection : public pattern::SingletonPattern<Reflection> {
	private:
		Reflection() = default;
	public:
		~Reflection() = default;
	public:
		base::return_code regist_factory(base::algorithm_code algorithm_code, std::shared_ptr<algorithm::BaseAlgorithm> baseAlgorithm) {
			__map[algorithm_code] = baseAlgorithm;
			return base::return_code::ok;
		}
		std::shared_ptr<algorithm::BaseAlgorithm> get_algorithm(base::algorithm_code algorithm_code) {
			auto it = __map.find(algorithm_code);
			if (__map.end() == it) { 
				return nullptr; 
			}
			else {
				return it->second;
			}
		}
	private:
		std::map<base::algorithm_code, std::shared_ptr<algorithm::BaseAlgorithm>> __map;
	private:
		friend pattern::SingletonPattern<Reflection>;
	};
}
