/*
 * @Description: 
 * @Autor: degawong
 * @Date: 2020-04-29 16:16:50
 * @LastEditors: degawong
 * @LastEditTime: 2020-05-12 15:04:22
 */
#pragma once

#include <deque>
#include <queue>
#include <mutex>
#include <chrono>
#include <future>
#include <random>
#include <vector>
#include <thread>
#include <atomic>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <execution>
#include <functional>
#include <condition_variable>

#include "module_time_clock.h"

namespace module {
	namespace for_each {
		void test_for_each() {
			auto tc = TimeClock();
			std::random_device rd;
			std::mt19937_64 mt_gengrator(rd());
			std::vector<double> operator_number(10000000);
			std::uniform_real_distribution<> uniform_real{ 0.0f, 100.0f };
			for (auto& ref : operator_number) {
				ref = uniform_real(mt_gengrator);
			}
			tc.time_duration();
			std::for_each(std::execution::seq, operator_number.begin(), operator_number.end(), [=](auto ref) mutable noexcept {
				ref = ref * ref;
			});
			std::cout << "std::execution::seq cost : " << tc.time_duration<std::chrono::milliseconds>() << "ms." << std::endl;
			std::for_each(std::execution::par, operator_number.begin(), operator_number.end(), [=](auto ref) mutable noexcept {
				ref = ref * ref;
			});
			std::cout << "std::execution::par cost : " << tc.time_duration<std::chrono::milliseconds>() << "ms." << std::endl;
		}
	}
	namespace stl {
		void test_clamp() {
			for (int i : {1, 3, 5, 7, 9}) {
				std::cout << std::clamp(i, 11, 9, [](int l, int r) {
					return l < r;
				}) << std::endl;
			}
		}
		void test_sample() {
			std::string in = "test_sample", out;
			std::sample(in.begin(), in.end(), std::back_inserter(out),
				5, std::mt19937{ std::random_device{}() });
			for (auto& ref : out) {
				std::cout << ref << "  ";
			}
			std::cout << std::endl;
		}
		void test_exception() {
			// current uncaught exceptions number
			auto exception_count = std::uncaught_exceptions();
		}
		void test_if_else() {
			if (auto var(6); false) {
			}
			else {
				var = 9;
			}
		}
		void test_at_exit_function() {
			std::atexit([]() {
				std::cout << "atexit function enter..." << std::endl;
			});
		}
	}
}
