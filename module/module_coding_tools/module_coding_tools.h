/*
 * @Description: 
 * @Autor: degawong
 * @Date: 2020-03-23 16:41:29
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-06-05 13:34:42
 */
#pragma once

#include <deque>
#include <queue>
#include <mutex>
#include <chrono>
#include <future>
#include <vector>
#include <thread>
#include <atomic>
#include <string>
#include <cstdarg>
#include <numeric>
#include <iostream>
#include <functional>
#include <condition_variable>

namespace module {

	std::string format_string(const char* format, ...) {
		va_list marker;
		std::string formation;
		va_start(marker, format);
		auto size_of_args = [&](const char* format, ...) {
			va_list args;
			va_start(args, format);
			auto size = vsnprintf(0, 0, format, args);
			va_end(args);
			return size;
		};
		auto size = size_of_args(format, marker);
		if (formation.capacity() <= size) {
			formation.reserve(size + 1);
		}
		if (0 > vsnprintf((char*)formation.data(), formation.capacity(), format, marker)) {
			return std::string();
		}
		va_end(marker);
		return formation;
	}
	// add the {} format implmentation
}
