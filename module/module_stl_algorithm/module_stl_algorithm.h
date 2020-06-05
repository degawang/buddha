/*
 * @Description: 
 * @Autor: degawong
 * @Date: 2020-04-29 16:16:50
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-05-25 10:57:10
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
#include <locale>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <execution>
#include <functional>
#include <condition_variable>

#include "module_time_clock.h"

namespace module {
	namespace test_concept {
		using namespace std::literals;

		//auto res = std::format("The answer is {}.", 42);
		//std::basic_format_args;
		//template <typename _func>
		//concept return_limit = requires(_func fun) {
		//	{fun()}->std::convertible_to<std::string>;
		//	std::is_function<fun>;
		//};
		//template <return_limit _func>
		//void print_string(_func func) {
		//	std::cout << func() << std::endl;
		//}
		std::string str1() {
			return "123";
		}
	}
	namespace for_each {
		void test_for_each() {
			auto tc = TimeClock();
			std::random_device rd;
			std::mt19937_64 mt_gengrator(rd());
			std::vector<double> operator_number(100);
			std::uniform_real_distribution<> uniform_real{ 0.0f, 100.0f };
			for (auto& ref : operator_number) {
				ref = uniform_real(mt_gengrator);
			}
			tc.time_duration();
			std::for_each(std::execution::seq, operator_number.begin(), operator_number.end(), [=](auto ref) mutable noexcept {
				std::cout << std::this_thread::get_id() << std::endl;
				ref = ref * ref;
			});
			std::cout << "std::execution::seq cost : " << tc.time_duration<std::chrono::milliseconds>() << "ms." << std::endl;
			// multi thread implementation
			std::for_each(std::execution::par, operator_number.begin(), operator_number.end(), [=](auto ref) mutable noexcept {
				std::cout << std::this_thread::get_id() << std::endl;
				ref = ref * ref;
			});
			std::cout << "std::execution::par cost : " << tc.time_duration<std::chrono::milliseconds>() << "ms." << std::endl;
		}
	}
	namespace stl {
		void test_clamp() {
			for (int i : {1, 3, 5, 7, 9}) {
				std::cout << std::clamp(i, 1, 9, [](int l, int r) {
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
		void test_for_allocator() {			
			std::allocator<int> a1;   // default allocator for ints
			int* a = a1.allocate(1);  // space for one int
			a1.construct(a, 7);       // construct the int
			std::cout << a[0] << '\n';
			a1.deallocate(a, 1);      // deallocate space for one int
			// default allocator for strings
			std::allocator<std::string> a2;
			// same, but obtained by rebinding from the type of a1
			decltype(a1)::rebind<std::string>::other a2_1;
			// same, but obtained by rebinding from the type of a1 via allocator_traits
			std::allocator_traits<decltype(a1)>::rebind_alloc<std::string> a2_2;
			std::string* s = a2.allocate(2); // space for 2 strings
			a2.construct(s, "foo");
			a2.construct(s + 1, "bar");
			std::cout << s[0] << ' ' << s[1] << '\n';
			a2.destroy(s);
			a2.destroy(s + 1);
			a2.deallocate(s, 2);
 		}
	}
}
