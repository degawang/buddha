/*
 * @Description: 
 * @Autor: degawong
 * @Date: 2020-04-15 17:24:12
 * @LastEditors: degawong
 * @LastEditTime: 2020-05-12 15:05:06
 */
#pragma once

namespace base {
	enum class info_type {
		dummy,
		console,
		std_file,
	};
	enum class return_code {
		success,
		unsupport,
		out_of_memory,
		path_not_exsit,
		file_open_error,
	};

	//template<typename _type_1, typename _type_2>
	//struct format_code;

	template<int _arg_1, int _arg_2, int _arg_3>
	struct format_code {
		enum { value = (((_arg_1 << 8) + _arg_2) << 8) + _arg_3 };
	};

	//template<int _arg_1, int _arg_2, int... _arg_rest>
	//struct format_code<std::integer_sequence<int, _arg_1, _arg_2>, std::integer_sequence<int, _arg_rest...>> {
	//	: format_code<format_code<std::integer_sequence<int, _arg_1, _arg_2>::, _arg_2>::value, _arg_rest...> {
	//	enum { value = format_code<format_code<>, _arg_rest...>::value };
	//};

	template<int _arg>
	struct format_value {
		enum { _1st = (_arg >> 8) };
		enum { _2nd = (_arg & 0x00ff) };
	};

	enum class image_format {
		image_format_bgr = format_code<1, 3, 0>::value,
		image_format_rgb = format_code<1, 3, 1>::value,
		image_format_yuv = format_code<1, 3, 2>::value,
		image_format_gray = format_code<1, 1, 0>::value,
		image_format_nv12 = format_code<2, 1, 0>::value,
		image_format_nv21 = format_code<2, 1, 1>::value,
	};
	enum class algorithm_code {
		base_algorithm,
		derived_algorithm,
	};

	// need to be removed
	template<typename _type = int>
	struct Range {
		_type end;
		_type begin;
		Range(_type _end_ = 0, _type _begin_ = 0) : end(_end_), begin(_begin_) {};
	};

	// add map && reduce
	template <typename _function, typename... _args>
	auto invoke_multi_time(_function function, _args... args) {
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

	class Noncopyable {
	protected:
		Noncopyable() = default;
		~Noncopyable() = default;
	private:
		Noncopyable(const Noncopyable&) = delete;
		const Noncopyable& operator=(const Noncopyable&) = delete;
	};
};