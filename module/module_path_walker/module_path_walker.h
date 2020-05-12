/*
 * @Description: 
 * @Autor: degawong
 * @Date: 2020-04-16 17:17:45
 * @LastEditors: degawong
 * @LastEditTime: 2020-05-12 15:04:30
 */
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
		template<typename _iter_type = std::filesystem::directory_iterator>
		std::vector<std::string> walk_path(
			const std::string directory,
			const std::string parameter = ".*\.(bmp|jpg|png)$") {
			auto res = __assert_path(directory);
			if (base::return_code::success != res) {
				return std::vector<std::string>();
			}
			if (directory != __directory) {
				__directory = directory;
				__parameter = parameter;
				__get_file_list<_iter_type>();
			}
			return std::vector<std::string>(__file_list);
		}
	private:
		PathWalker() = default;
	private:
		template<typename _iter_type>
		void __get_file_list() {
			std::regex regex(__parameter);
			std::vector<std::string>().swap(__file_list);
			if (std::filesystem::is_directory(__directory)) {
				for (auto ref : _iter_type(__directory)) {
					auto file_name(ref.path().generic_string());
					if (std::regex_match(file_name, regex)) {
						__file_list.push_back(file_name);
					}
				}
			}
		}
	private:
		base::return_code __assert_path(std::string path) {
			if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
				return base::return_code::success;
			}
			else {
				return base::return_code::path_not_exsit;
			}
		}
	private:
		std::string __directory;
		std::string __parameter;
		std::vector<std::string> __file_list;
	private:
		friend pattern::SingletonPattern<PathWalker>;
	};
}