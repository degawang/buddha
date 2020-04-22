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

	template<typename _type = int>
	struct Range {
		_type end;
		_type begin;
		Range(_type _end_ = 0, _type _begin_ = 0) : end(_end_), begin(_begin_) {};
	};

	template <typename _function, typename... _args>
	auto invoke_multi_time(_function func, _args... args) {
		std::initializer_list<int>{ ((function(std::forward<_args>(args))), 0)...};
	}

	template <typename _type>
	_type min(_type&& value) {
		return value;
	}

	template <typename _type>
	_type min(_type&& first, _type&& second) {
		return (_type)(first > second ? second : first);
	}

	template <typename _type, typename... _rest>
	_type min(_type&& head, _rest&&... rest) {
		return min(head, min(rest...));
	}

	template <typename _type>
	_type max(_type&& value) {
		return value;
	}

	template <typename _type>
	_type max(_type&& first, _type&& second) {
		return (_type)(first < second ? second : first);
	}

	template <typename _type, typename... _rest>
	_type max(_type&& head, _rest&&... rest) {
		return max(head, max(rest...));
	}

	template <typename _type>
	_type clamp(_type&& value, _type&& minimum, _type&& maximum) {
		return (_type)(min(max(value, minimum), maximum));
	}

	template <typename... _type>
	void dummy(_type&&... type) {}

	template <typename _type, typename... _args>
	_type sum(_args&&... args) {
		return (_type)(args + ...);
	}

	auto any_equel = [](auto&& input, auto&&... args) -> bool {
		return ((args == input) || ...);
	};
	auto any_lower = [](auto&& input, auto&&... args) -> bool {
		return ((args > input) && ...);
	};
	auto any_larger = [](auto&& input, auto&&... args) -> bool {
		return ((args < input) && ...);
	};
	auto every_eque = [](auto&& input, auto&&... args) -> bool {
		return ((args == input) && ...);
	};
	auto every_lower = [](auto&& input, auto&&... args) -> bool {
		return ((args > input) && ...);
	};
	auto every_larger = [](auto&& input, auto&&... args) -> bool {
		return ((args < input) && ...);
	};


};