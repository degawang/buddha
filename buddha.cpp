
#include <atomic>
#include <thread>
#include <vector>
#include <fstream>
#include <iostream>

#include "base/base.h"
#include "algorithm_base.h"
#include "module_time_clock.h"
#include "algorithm_derived.h"
#include "module_reflection.h"
#include "module_information.h"
#include "module_path_walker.h"
#include "module_memory_pool.h"
#include "module_thread_pool.h"

// test for memory leak
#include "vld.h"

int main() {
	//log info
	module::Information<> a("main", "_1st ", 1, " _2nd ", "2");
	//std::cout << typeid(module::InforLevel<base::infor_type::console>().get_info_type()).name() << std::endl;
	//std::cout << typeid(module::InforLevel<base::infor_type::std_file>().get_info_type()).name() << std::endl;
	//auto mo = module::InforAdaptor(aa);
	//mo.out_formation("asa", 1);
	//aa.close();
	//auto mo = module::Information(std::cout);
	//auto d = module::Dummy();
	//d << "a" << 1 << 2 << std::endl;
	//auto a = op<std::ostream>(2,std::move(std::cout));
	//a("a",1,2,3,4,"b");
	//auto tc = module::TimeClock<std::ostream>(std::string("main"), std::cout);
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
	//memory pool
	//auto mp_int = module::MemoryPool<int>::get_instance();
	//auto ap = mp_int->new_object();
	//mp_int->delete_object(ap);
	// path walker
	//auto path_walker(module::PathWalker::get_instance());
	//auto file_list(path_walker->walk_path<std::filesystem::directory_iterator>("f:/image/arcsoft_lut"));
	//for (auto ref : file_list) {
	//	std::cout << ref.c_str() << std::endl;
	//}
	// reflection
	//auto rf(module::Reflection<algorithm::BaseAlgorithm, base::algorithm_code>::get_instance());
	//rf->regist_factory(base::algorithm_code::derived_algorithm, algorithm::DrivedAlgorithm::get_instance());
	//auto alg = rf->get_algorithm(base::algorithm_code::derived_algorithm);
	//alg->hello();
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

    return 0;
}