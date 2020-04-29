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

namespace module {
	namespace merge_sort {
		template<int... _args>
		struct int_list {};

		//std::integer_sequence<int>;
		template<int... _args>
		void print(int_list<_args...>) {
			constexpr int args[] = { _args... };
			::std::copy(::std::begin(args), ::std::end(args), ::std::ostream_iterator<int>(::std::cout, " "));
			::std::endl(::std::cout);
		}

		// input serialize segment
		template<typename _type_1, typename _type_2>
		struct devide;

		template<int... _1st>
		struct devide<int_list<_1st...>, int_list<>>
			: ::std::pair<int_list<_1st...>, int_list<>>::type
		{};

		template<int... _1st, int _mid, int... _2nd>
		struct devide<int_list<_1st...>, int_list<_mid, _2nd...>>
			: ::std::conditional<(sizeof...(_1st) < 1 + sizeof...(_2nd)),
			devide<int_list<_1st..., _mid>, int_list<_2nd...>>,
			::std::pair<int_list<_1st...>, int_list<_mid, _2nd...>>>::type
		{};

		// merge segment
		template<typename _1st, typename _2nd, typename _3rd>
		struct merge;

		template<int... _1st>
		struct merge<int_list<_1st...>, int_list<>, int_list<>>
			: ::std::enable_if<true, int_list<_1st...>>
		{};

		template<int... _1st, int _2_1st, int... _2_rest>
		struct merge<int_list<_1st...>, int_list<_2_1st, _2_rest...>, int_list<>>
			: ::std::enable_if<true, int_list<_1st..., _2_1st, _2_rest...>>
		{};

		template<int... _1st, int _3_1st, int... _3_rest>
		struct merge<int_list<_1st...>, int_list<>, int_list<_3_1st, _3_rest...>>
			: ::std::enable_if<true, int_list<_1st..., _3_1st, _3_rest...>>
		{};

		template<int... _1st, int _2_1st, int... _2_rest, int _3_1st, int... _3_rest>
		struct merge<int_list<_1st...>, int_list<_2_1st, _2_rest...>, int_list<_3_1st, _3_rest...>>
			: ::std::conditional<_2_1st <= _3_1st,
								merge<int_list<_1st..., _2_1st>, int_list<_2_rest...>, int_list<_3_1st, _3_rest...>>,
								merge<int_list<_1st..., _3_1st>, int_list<_2_1st, _2_rest...>, int_list<_3_rest...>>
			>::type
		{};

		// merge sort
		template<typename _type>
		struct sort;

		template<>
		struct sort<int_list<>>
			: ::std::enable_if<true, int_list<>>
		{};

		template<int _1st>
		struct sort<int_list<_1st>>
			: ::std::enable_if<true, int_list<_1st>>
		{};

		template<int _1st, int _2nd>
		struct sort<int_list<_1st, _2nd>>
			: ::std::conditional< _1st < _2nd,
								int_list<_1st, _2nd>,
								int_list<_2nd, _1st>>
		{};

		template<int... _1st>
		struct sort<int_list<_1st...>>
			: merge<int_list<>,
					typename sort<typename devide<int_list<>, int_list<_1st...>>::first_type>::type,
					typename sort<typename devide<int_list<>, int_list<_1st...>>::second_type>::type>
		{};
	}
}
