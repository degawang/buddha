
#include <atomic>
#include <thread>
#include <iostream>

#include "module_thread_pool.h"

int main() {

	std::mutex mtx;
	auto tp(module::ThreadPool::get_instance(13));
	std::vector<std::future<void>> vr;

	for (int i = 0; i <= 25; ++i)
	{
		auto ans = tp->commit_task([&mtx]() {
			std::cout << "hello world!" << std::endl;
			return;
		});
		vr.push_back(std::move(ans));
	}
	for (auto& ref : vr) {
		ref.get();
	}

    return 0;
}