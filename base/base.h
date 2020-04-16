#pragma once

namespace base {
	enum class return_code {
		ok = 0,
		out_of_memory,
		path_not_exsit,
		file_read_error,
		file_write_error,
	};
	enum class algorithm_code {
		base_algorithm = 0,
		derived_algorithm = 1,
	};
};