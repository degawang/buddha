/*
 * @Author: degawong 
 * @Date: 2020-03-23 16:41:29
 * @LastEditTime: 2020-05-25 10:36:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: buddha/buddha.cpp
 */

#include <atomic>
//#include <format>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "base/base.h"
#include "algorithm_base.h"
#include "module_time_clock.h"
#include "algorithm_derived.h"
#include "module_reflection.h"
#include "module_information.h"
#include "module_image_tools.h"
#include "module_path_walker.h"
#include "module_memory_pool.h"
#include "module_thread_pool.h"
#include "module_coding_tools.h"
#include "module_meta_program.h"
#include "module_stl_algorithm.h"

// test for memory leak
//#include "vld.h"

template<int... _args>
struct intger_number {};

template<typename _type_1, typename _type_2>
struct format;

template<int _arg_1, int _arg_2>
struct format<intger_number<_arg_1>, intger_number<_arg_2>> {
	enum { value = (_arg_1 << 8) + _arg_2 };
};

template<int... _arg_rest>
struct format<intger_number<>, intger_number<_arg_rest...>> {
	enum { value = format<intger_number<_arg_rest...>>::value };
};

//template<int _arg_1, int... _arg_rest>
//struct format<intger_number<_arg_1>, intger_number<_arg_rest...>> {
//	enum { value = format<intger_number<_arg_1>, intger_number<_arg_rest...>>::value };
//};

template<int _arg_1, int _arg_2, int... _arg_rest>
struct format<intger_number<_arg_1, _arg_2>, intger_number<_arg_rest...>> {
	enum { value = format<intger_number<(_arg_1 << 8) + _arg_2>, intger_number<_arg_rest...>>::value };
};

template<int... _args>
void printline(intger_number<_args...>) {
	constexpr int args[] = { _args... };
	::std::copy(::std::begin(args), ::std::end(args), ::std::ostream_iterator<int>(::std::cout, " "));
	::std::endl(::std::cout);
}

//template<typename T, class F>
//auto operator|(T&& param, const F& f) -> decltype(f(std::forward<T>(param)))
//{
//	return f(std::forward<T>(param));
//}

class test {
public:
	operator int() const {
		std::cout << "anther kind of writting style" << std::endl;
		return 0;
	}
};

//template<int _value>
//class to_binary {
//	static unsigned const value = to_binary<_value / 10>::value * 2;
//};
//
//template<>
//class to_binary<0> {
//	static unsigned const value = 0;
//};

template<int _value>
struct to_decimal {
	static unsigned const value = to_decimal<_value / 10>::value * 2 + _value % 10;
};

template<>
struct to_decimal<0> {
	static unsigned const value = 0;
};

//template<typename T>
//concept bool Stringable = requires(T a) {
//	{a.to_string()} -> std::string;
//};



int main() {
	{
		using namespace module::base_meta;
		//std::cout << sum_pow<2, 2>::ret << std::endl;;
		//copy(l.rbegin(), l.rend(), ostream_iterator<int>{cout, ", "});
		auto f = module::format_string("o_%s_%d_o", "some", 88);
		std::cout << f.data() << std::endl;
	}
	{
		//using namespace module::test_concept;
		//print_string(&str1);
		//auto a = to_decimal<10>::value;
		//auto ret = test()();
		//std::cout << std::format("Hello {}!\n", "world");
	}
	{
		//functor
		//using namespace base;
		//auto o = [=](bool a) {
		//	std::cout << "functor " << std::endl;
		//	return int(7);
		//};
		//auto add_one = [](auto a) {
		//	std::cout << a << std::endl;
		//	return 1 + a; 
		//};
		//0 | o | add_one;
	}
	{
		// test for log info
		//auto res = module::Information<INFO_TYPE>(__FUNCTION__, "_1st ", 1, " _2nd ", "2");
		//res.write_infomation("stringstream");
	}
	{
		// test for mat
		using namespace module::colorspace;
		//Mat a(10, 10, base::format_code<1,3,0>::value);
		//a.set_value(0);
		//auto b = a;
		//auto c(b.rect(3, 5, 3, 5));
		//c.set_value(1);
		//auto cc = c.copy();
		//auto d(b.rect(5, 8, 5, 8));
		//d.set_value(9);
		//Mat bgr(10, 10, 66304);
		//bgr.set_value(64);
		//bgr = Mat(300, 300, 63306);
		//Mat rgb(10, 10, 66305);
		//rgb.set_value(64);
		//Mat yuv(10, 10, 66306);
		//yuv.set_value(64);
		//Mat nv12(10, 10, 131328);
		//Mat nv21(10, 10, 131329);
		//module::MatData<float, 128> t(10, 10, 131328);
		//auto res = module::colorspace::color_convert(bgr, yuv);

		//module::Tensor<unsigned char, 64> tensor_0(5, 5, 1);
		//module::Tensor<float, 64> tensor_1(5, 5, 2);
		//module::Tensor<double, 64> tensor_2(5, 5, 3);
		//tensor_0 = module::Tensor(2, 2, 2);
	}
	{
		// meta
		//std::cout << format<intger_number<1>, intger_number<1, 2, 3>>::value << std::endl;
		//printline(intger_number<format<1, 2>::value>{})
	}
	{
		//aligned
		//using namespace base;
		//std::cout << format_value<(format_code<1, 1>::value)>::_1st << std::endl;
		//std::cout << format_value<(format_code<1, 2>::value)>::_2nd << std::endl;
	}
	{
		//stl algorithm
		//module::for_each::test_for_each();
		//module::stl::test_clamp();
		//module::stl::test_sample();
		//module::stl::test_at_exit_function();
		//std::vector<int> vi; __cplusplus;
		//std::vector<int>::iterator a;
		//std::cout << (&vi) << std::endl;
		//std::cout << std::addressof(vi) << std::endl;
	}
	{
		//log info 
		//auto res = module::Information<INFO_TYPE>(__FUNCTION__, "_1st ", 1, " _2nd ", "2");
	}
	{
		//module image tools
		//using namespace base;
		//std::cout<<format_code<1, 2>::value;
		//auto res = int(format_code<1, 2>::value);
		//auto res_path = std::filesystem::absolute("Debug/arcsoft_face_detection.dll").generic_string();
		//std::cout << res_path.c_str() << std::endl;
	}
	{
		//// meta program
		//using namespace module::merge_sort;
		//print(sort<int_list<88, 52, 3, 48, 15, 99, 20>>::type());
	}
	{
		// cosmos
		//using namespace cosmos;
		//using my_list = list<int, bool>;
		//static_assert(std::is_same<transform<std::add_pointer_t, my_list>, list<int*, bool*>>::value, "no");
		//using my_list1 = push_back<my_list, char, double>;
		//static_assert(std::is_same<my_list1, list<int, bool, char, double>>::value, "no");
		//using my_list4 = list<bool, char, char, double, char>;
		//static_assert(std::is_same<erase_dupl<my_list4>, list<bool, char, double>>::value, "no");
		//static_assert(std::is_same<erase_front<1, my_list4>, list<char, char, double, char>>::value, "no");
		//static_assert(std::is_same<erase_front<2, my_list4>, list<char, double, char>>::value, "no");

		//static_assert(std::is_same<slice<1, 3, list<bool, char, char, double, char>>, list<char, char>>::value, "no");
		//static_assert(std::is_same<slice<0, 3, list<bool, char, char, double, char>>, list<bool, char, char>>::value, "no");

		//static_assert(std::is_same<replace_at<2, short, my_list1>, list<int, bool, short, double>>::value, "no");

		//using my_list5 = list<bool, char, char, int, double, int, double, char>;
		//static_assert(std::is_same<erase_dupl<my_list5>, list<bool, char, int, double>>::value, "no");

		//static_assert(std::is_same<replace_all<short, char, my_list5>, list<bool, short, short, int, double, int, double, short>>::value, "no");

		//static_assert(std::is_same<erase_all<char, my_list4>, list<bool, double>>::value, "no");
		//static_assert(std::is_same<erase_all<char, my_list>, my_list>::value, "no");

		//static_assert(std::is_same<erase_type<char, my_list1>, list<int, bool, double>>::value, "no");
		//static_assert(size<my_list>::value == 2, "no");
		//using my_list3 = pop_front<my_list1>;
		//static_assert(std::is_same<my_list3, list<bool, char, double>>::value, "no");
		//static_assert(std::is_same<reverse<my_list3>, list<double, char, bool>>::value, "no");
		//using ty = at<1, my_list>;
		//static_assert(std::is_same<at<2, my_list1>, char>::value, "no");
		//static_assert(std::is_same<front<my_list>, int>::value, "no");
		//static_assert(std::is_same<back<my_list>, bool>::value, "no");
		//using spl = split<3, my_list1 >;

		//static_assert(index_of<char, list<bool, char, double>>::value == 1, "no");
		//static_assert(index_of<double, list<bool, char, double>>::value == 2, "no");
		//static_assert(index_of<bool, list<bool, char, double>>::value == 0, "no");
		//static_assert(index_of<short, list<bool, char, double>>::value == -1, "no");

		//static_assert(std::is_same<as_tuple<my_list>, std::tuple<int, bool>>::value, "no");
		//static_assert(std::is_same<as_pair<my_list>, std::pair<int, bool>>::value, "no");
		//using spl1 = split<3, std::tuple<int, bool, char, double>, std::tuple<> >;
		//std::cout << typeid(spl1).name() << std::endl;
		//using spl2 = split1<3, list<int, bool, char, double>>;
		//std::cout << typeid(spl2).name() << std::endl;
		//static_assert(std::is_same<split<3, my_list1>, list<int, bool, char>>::value, "no");
		//static_assert(std::is_same<split1<3, my_list1>, list<int, bool, char>>::value, "no");
		//static_assert(std::is_same<pop_back<my_list1>, list<int, bool, char>>::value, "no");
		//static_assert(std::is_same<erase_at<2, my_list1>, list<int, bool, double>>::value, "no");
		//static_assert(std::is_same<append<my_list, list<char, double>>, my_list1>::value, "no");

		//using my_list2 = push_front<my_list, char, double>;
		//static_assert(std::is_same<my_list2, list<char, double, int, bool>>::value, "no");

		//using my_tp = push_back<std::tuple<int, double>, char, bool>;
		//static_assert(std::is_same<my_tp, std::tuple<int, double, char, bool >>::value, "no");
	}
	{
		//parallel for
		//module::parallel_for(base::Range{ 100,10 }, [&](int a,int b) {
		//	if (a < 20) {
		//		std::cout << a << std::endl;
		//		std::cout << aa << std::endl;
		//	}
		//	return 0;
		//});
		//std::cout << "outer : " << aa << std::endl;
		//parallel for each
		//module::parallel_for_each(0,10,[&](int a) {
		//	if (a < 20) {
		//		std::cout << a << std::endl;
		//	}
		//	return 0;
		//});
	}
	{
		//memory pool
		//auto mp_int = module::MemoryPool<int>::get_instance();
		//auto ap = mp_int->new_object();
		//mp_int->delete_object(ap);
	}
	{
		// path walker
		//auto path_walker(module::PathWalker::get_instance());
		//auto file_list(path_walker->walk_path<std::filesystem::directory_iterator>("f:/image/arcsoft_lut"));
		//for (auto ref : file_list) {
		//	std::cout << ref.c_str() << std::endl;
		//}
	}
	{
		// reflection
		//auto rf(module::Reflection<algorithm::BaseAlgorithm, base::algorithm_code>::get_instance());
		//rf->regist_factory(base::algorithm_code::derived_algorithm, algorithm::DrivedAlgorithm::get_instance());
		//auto alg = rf->get_algorithm(base::algorithm_code::derived_algorithm);
		//alg->hello();
	} 
	{
		// thread pool
		//std::vector<std::future<void>> v;
		//auto tp(module::ThreadPool::get_instance(1));
		//for (size_t i = 0; i <= 16; ++i) {
		//	v.push_back(std::move(
		//		tp->commit_task([]() {
		//		std::this_thread::sleep_for(std::chrono::microseconds(20));
		//		std::cout << std::this_thread::get_id() << std::endl;
		//		return;
		//	})));
		//}
		//for (auto& ref : v) {
		//	ref.get();
		//}
	}
    return 0;
}