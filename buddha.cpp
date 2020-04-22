
#include <atomic>
#include <thread>
#include <vector>
#include <iostream>

#include "base/base.h"
#include "algorithm_base.h"
#include "module_reflection.h"
#include "module_path_walker.h"
#include "module_memory_pool.h"
#include "module_thread_pool.h"
#include "pattern_singleton_pattern.h"

// test for memory leak
#include "vld.h"

class DrivedAlgorithm : public algorithm::BaseAlgorithm, public pattern::SingletonPattern<DrivedAlgorithm> {
public:
	virtual void hello() const override {
		std::cout << "hello DrivedAlgorithm !" << std::endl;
	}
};

int main() {
	//int aa = 50;
	//module::parallel_for(base::Range{ 100,10 }, [&](int a,int b) {
	//	if (a < 20) {
	//		std::cout << a << std::endl;
	//		aa = 9;
	//		std::cout << aa << std::endl;
	//	}
	//	return 0;
	//});
	//std::cout << "outer : " << aa << std::endl;
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
	//auto file_list(path_walker->non_recursive_walk(""));
	//for (auto ref : file_list) {
	//	std::cout << ref.c_str() << std::endl;
	//}
	// reflection
	//auto rf(module::Reflection<algorithm::BaseAlgorithm, base::algorithm_code>::get_instance());
	//rf->regist_factory(base::algorithm_code::base_algorithm, DrivedAlgorithm::get_instance());
	//auto alg = rf->get_algorithm(base::algorithm_code::base_algorithm);
	//alg->hello();
	// thread pool
	//std::vector<std::future<void>> v;
	//auto tp(module::ThreadPool::get_instance(8));
	//for (size_t i = 0; i <= 16; ++i) {
	//	v.push_back(std::move(
	//		tp->commit_task([]() {
	//		std::this_thread::sleep_for(std::chrono::microseconds(20));
	//		std::cout << "hello thread pool!" << std::endl;
	//		return;
	//	})));
	//}
	//for (auto& ref : v) {
	//	ref.get();
	//}

    return 0;
}