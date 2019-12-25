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

#include <assert.h>

namespace module {
	auto any_equel = [](auto&& input, auto&&... args) -> bool {
		return ((args == input) || ...);
	};
	auto any_lower = [](auto&& input, auto&&... args) -> bool {
		return ((args > input) && ...);
	};
	auto any_larger = [](auto&& input, auto&&... args) -> bool {
		return ((args < input) && ...);
	};
	auto every_eque = [](auto&& input, auto&&... args) -> bool {
		return ((args == input) && ...);
	};
	auto every_lower = [](auto&& input, auto&&... args) -> bool {
		return ((args > input) && ...);
	};
	auto every_larger = [](auto&& input, auto&&... args) -> bool {
		return ((args < input) && ...);
	};

	template <typename... _type>
	void dummy(_type&&... type) {}

	template <typename return_type, typename... _args>
	return_type sum(_args&&... args) {
		return (return_type)(args + ...);
	}

	// 一个函数使用多个参数多次调用
	template <typename _func, typename... _args>
	auto invoke_multi_time(_func func, _args... args) {
		std::initializer_list<int>{ ((func(std::forward<_args>(args))), 0)...};
	}

	template <typename _1st>
	_1st min(_1st&& first) {
		return first;
	}

	template <typename _1st, typename _2nd>
	_1st min(_1st&& first, _2nd&& second) {
		return (_1st)(first > second ? second : first);
	}

	template <typename _1st, typename... _rest>
	_1st min(_1st&& first, _rest&&... rest) {
		return min(first, min(rest...));
	}

	template <typename _1st>
	_1st max(_1st&& first) {
		return first;
	}

	template <typename _1st, typename _2nd>
	_1st max(_1st&& first, _2nd&& second) {
		return (_1st)(first < second ? second : first);
	}

	template <typename _1st, typename... _rest>
	_1st max(_1st&& first, _rest&&... rest) {
		return max(first, max(rest...));
	}

	template <typename _inout, typename _in_1, typename _in_2>
	_inout clamp(_inout&& inout, _in_1&& in_1, _in_2&& in_2) {
		assert(in_1 < in_2);
		return (_inout)(min(max(inout, in_1), in_2));
	}
}