
#include "module_path_walker.h"

std::vector<std::string> module::PathWalker::recursive_walk(const std::string directory, const std::string parameter) {
	auto res = __assert_path(directory);
	if (base::return_code::success != res) {
		return std::vector<std::string>();
	}
	if (directory != __directory) {
		__directory = directory;
		__parameter = parameter;
		__recursive_get_file_list();
	}
	return std::vector<std::string>(__file_list);
}
std::vector<std::string> module::PathWalker::non_recursive_walk(const std::string directory, const std::string parameter) {
	auto res = __assert_path(directory);
	if (base::return_code::success != res) {
		return std::vector<std::string>();
	}
	if (directory != __directory) {
		__directory = directory;
		__parameter = parameter;
		__non_recursive_get_file_list();
	}
	return std::vector<std::string>(__file_list);
}
void module::PathWalker::__recursive_get_file_list() {
	std::regex regex(__parameter);
	std::vector<std::string>().swap(__file_list);
	if (std::filesystem::is_directory(__directory)) {
		for (auto ref : std::filesystem::recursive_directory_iterator(__directory)) {
			auto file_name(ref.path().generic_string());
			if (std::regex_match(file_name, regex)) {
				__file_list.push_back(file_name);
			}
		}
	}
}
void module::PathWalker::__non_recursive_get_file_list() {
	std::regex regex(__parameter);
	std::vector<std::string>().swap(__file_list);
	if (std::filesystem::is_directory(__directory)) {
		for (auto ref : std::filesystem::directory_iterator(__directory)) {
			auto file_name(ref.path().generic_string());
			if (std::regex_match(file_name, regex)) {
				__file_list.push_back(file_name);
			}
		}
	}
}
base::return_code module::PathWalker::__assert_path(std::string path) {
	if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
		return base::return_code::success;
	}
	else {
		return base::return_code::path_not_exsit;
	}
}



