#pragma once

namespace base {
	enum class return_code {
		success,
		out_of_memory,
		path_not_exsit,
		file_open_error,
	};
	enum class image_format {
		image_format_bgr,
		image_format_rgb,
		image_format_yuv,
		image_format_gray,
		image_format_nv12,
		image_format_nv21,
	};
	enum class algorithm_code {
		base_algorithm,
		derived_algorithm,
	};
};