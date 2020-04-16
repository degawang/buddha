
#include <atomic>
#include <thread>
#include <iostream>

#include "algorithm_base.h"
#include "module_reflection.h"
#include "module_path_walker.h"
#include "module_thread_pool.h"
#include "pattern_singleton_pattern.h"

// test for memory leak
// #include "vld.h"

class DrivedAlgorithm : public algorithm::BaseAlgorithm, public pattern::SingletonPattern<DrivedAlgorithm> {
public:
	virtual void hello() {
		std::cout << "hello DrivedAlgorithm !" << std::endl;
	}
};

int main() {

	// path walker
	auto path_walker(module::PathWalker::get_instance());
	auto file_list(path_walker->non_recursive_walk("f:/image/hair"));
	for (auto ref : file_list) {
		std::cout << ref.c_str() << std::endl;
	}
	// reflection
	auto rf(module::Reflection::get_instance());
	rf->regist_factory(base::algorithm_code::base_algorithm, DrivedAlgorithm::get_instance());
	auto alg = rf->get_algorithm(base::algorithm_code::base_algorithm);
	alg->hello();
	// thread pool
	std::vector<std::future<void>> v;
	auto tp(module::ThreadPool::get_instance(8));
	for (size_t i = 0; i <= 16; ++i) {
		v.push_back(std::move(
			tp->commit_task([]() {
			std::this_thread::sleep_for(std::chrono::microseconds(20));
			std::cout << "hello thread pool!" << std::endl;
			return;
		})));
	}
	for (auto& ref : v) {
		ref.get();
	}

    return 0;
}