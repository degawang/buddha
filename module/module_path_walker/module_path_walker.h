#pragma once

#include <deque>
#include <regex>
#include <mutex>
#include <queue>
#include <atomic>
#include <future>
#include <thread>
#include <vector>
#include <numeric>
#include <iostream>
#include <filesystem>
#include <functional>
#include <condition_variable>

#include "../../base/base.h"
#include "pattern_singleton_pattern.h"

namespace module{

	class PathWalker : public pattern::SingletonPattern<PathWalker> {
	public:
		~PathWalker() {
			std::vector<std::string>().swap(__file_list);
		};
	public:
		std::vector<std::string> recursive_walk(
			const std::string directory,
			const std::string parameter = ".*\.(bmp|jpg|png)$");
		std::vector<std::string> non_recursive_walk(
			const std::string directory, 
			const std::string parameter = ".*\.(bmp|jpg|png)$");
	private:
		PathWalker() = default;
	private:
		void __recursive_get_file_list();
		void __non_recursive_get_file_list();
	private:
		base::return_code assert_path(std::string);
	private:
		std::string __directory;
		std::string __parameter;
		std::vector<std::string> __file_list;
	private:
		friend pattern::SingletonPattern<PathWalker>;
	};
}