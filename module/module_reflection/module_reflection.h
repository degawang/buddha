/*
 * @Description: 
 * @Autor: degawong
 * @Date: 2020-04-15 15:28:40
 * @LastEditors: degawong
 * @LastEditTime: 2020-05-12 15:04:26
 */
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
	template<typename _algorithm_type = algorithm::BaseAlgorithm, typename _algorithm_code = base::algorithm_code>
	class Reflection : public pattern::SingletonPattern<Reflection<_algorithm_type, _algorithm_code>> {
	private:
		Reflection() = default;
	public:
		~Reflection() = default;
	public:
		base::return_code regist_factory(_algorithm_code algorithm_code, std::shared_ptr<_algorithm_type> baseAlgorithm) {
			__map[algorithm_code] = baseAlgorithm;
			return base::return_code::success;
		}
		std::shared_ptr<_algorithm_type> get_algorithm(_algorithm_code algorithm_code) {
			auto it = __map.find(algorithm_code);
			if (__map.end() == it) { 
				return nullptr; 
			}
			else {
				return it->second;
			}
		}
	private:
		std::map<_algorithm_code, std::shared_ptr<_algorithm_type>> __map;
	private:
		friend pattern::SingletonPattern<Reflection>;
	};
}
