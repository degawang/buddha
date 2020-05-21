/*
 * @Author: your name
 * @Date: 2020-04-30 10:39:53
 * @LastEditTime: 2020-05-16 18:30:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \buddha\module\module_image_tools\module_image_tools.h
 */
#pragma once

#include <deque>
#include <queue>
#include <mutex>
#include <chrono>
#include <future>
#include <vector>
#include <thread>
#include <atomic>
#include <cstdlib>
#include <numeric>
#include <iostream>
#include <functional>
#include <condition_variable>

#include "../../base/base.h"

// BT.601 YUV to RGB reference
//  R = (Y - 16) * 1.164              - V * -1.596
//  G = (Y - 16) * 1.164 - U *  0.391 - V *  0.813
//  B = (Y - 16) * 1.164 - U * -2.018

// Y contribution to R,G,B.  Scale and bias.
// TODO(fbarchard): Consider moving constants into a common header.
#define YG 18997 /* round(1.164 * 64 * 256 * 256 / 257) */
#define YGB -1160 /* 1.164 * 64 * -16 + 64 / 2 */

// U and V contributions to R,G,B.
#define UB -128 /* max(-128, round(-2.018 * 64)) */
#define UG 25 /* round(0.391 * 64) */
#define VG 52 /* round(0.813 * 64) */
#define VR -102 /* round(-1.596 * 64) */

// Bias values to subtract 16 from Y and 128 from U and V.
#define BB (UB * 128 + YGB)
#define BG (UG * 128 + VG * 128 + YGB)
#define BR (VR * 128 + YGB)

namespace module {

	template<class _derived>
	class ReferCount {
	public:
		//ReferCount() : __shareable(_shareable), __refer_count(nullptr) {
		//}
		//ReferCount(int* _ref_count) :__shareable(_shareable), __refer_count(_ref_count) {
		//}
		//virtual ~ReferCount() {
		//}
	public:
		//ReferCount(ReferCount&& anther) 
		//	: __refer_count(anther.__refer_count),
		//	__shareable(anther.__shareable) {
		//	anther.__refer_count = nullptr;
		//}
		//ReferCount(const ReferCount& anther)
		//	: __refer_count(anther.__refer_count),
		//	__shareable(anther.__shareable) {
		//	__add_ref_count();
		//}
		//ReferCount& operator= (const ReferCount& anther) {
		//	if (&anther != this) {
		//		__dec_ref_count();
		//		anther.__add_ref_count();
		//		__shareable = anther.__shareable;
		//		__refer_count = anther.__refer_count;
		//	}
		//	return *this;
		//}
		//ReferCount& operator = (ReferCount&& anther) {
		//	if (&anther != this) {
		//		__dec_ref_count();
		//		__shareable = anther.__shareable;
		//		__refer_count = anther.__refer_count;
		//		anther.__refer_count = nullptr;
		//	}
		//	return *this;
		//}
	protected:		
		void _shallow_clean() {
			__release();
		}
		int* _get_refer() {
			return __refer_count;
		}
		int* _get_refer() const {
			return __refer_count;
		}
		void _add_ref_count() {
			//std::lock_guard<std::recursive_mutex> lock(__mutex);
			if (__refer_count) {
				(*__refer_count)++;
			}
		}
		void _dec_ref_count() {
			//std::lock_guard<std::recursive_mutex> lock(__mutex);
			if (__derived().__shareable) {
				if (__refer_count) {
					if (((*__refer_count)--) == 1) {
						__uninit();
					}
					__release();
				}
			}
		}
		void _init(int* ref_count) {
			__refer_count = ref_count;
		}
		//void _init_with(int* ref_count) {
		//	__refer_count = ref_count;
		//}
		//void _init_with(int count, int* ref_count) {
		//	*ref_count = count;
		//	__refer_count = ref_count;
		//}
	private:
		_derived& __derived() { 
			return *static_cast<_derived*>(this); 
		}
		const _derived& __derived() const {
			return *static_cast<const _derived*>(this); 
		}
		void __release() {
			__refer_count = nullptr;
			__derived().__shallow_clean();
		}
		void __uninit() {
			delete __refer_count;
			__derived().__dellocator();
		}
	protected:
		int* __refer_count;
		//std::recursive_mutex __mutex;
	};

	template<typename _data_type = unsigned char, int _align_size = 64>
	class MatData : public ReferCount<MatData<_data_type, _align_size>> {
	public:
		MatData(bool shareable = true) : __shareable(shareable) {
			_shallow_clean();
			if (__shareable) {
				_init(new int(1));
			}
		}
		MatData(int width, int height, int code_format) noexcept
			: __width(width), __height(height), __pitch{ 0 }, __shareable(true), __chunck_size{ 0 }, __code_format(code_format), __data{ nullptr } {
			_init(new int(1));
			__allocator();
		}
		~MatData() {
			_dec_ref_count();
		}
	public:
		MatData(MatData&& object) noexcept {
			_shallow_clean();
			_init(object._get_refer());
			__width = object.__width;
			__height = object.__height;
			__shareable = object.__shareable;
			__code_format = object.__code_format;
#pragma unroll
			for (size_t i = 0; i < 4; ++i) {
				__data[i] = object.__data[i];
				__pitch[i] = object.__pitch[i];
			}
			object._shallow_clean();
		}
		MatData(const MatData& object) noexcept {
			_shallow_clean();
			_init(object._get_refer());
			__width = object.__width;
			__height = object.__height;
			__shareable = object.__shareable;
			__code_format = object.__code_format;
#pragma unroll
			for (size_t i = 0; i < 4; ++i) {
				__data[i] = object.__data[i];
				__pitch[i] = object.__pitch[i];
			}
			if (__shareable) {
				_add_ref_count();
			}
		}
		// one kind of copy constructor
		//MatData(const MatData& object) noexcept {
		//	_shallow_clean();
		//	_init(new int(1));
		//	__width = object.__width;
		//	__height = object.__height;
		//	__shareable = object.__shareable;
		//	__code_format = object.__code_format;
		//	__allocator();
		//	__copy_data(object);
		//}
	public:
		MatData& operator= (MatData&& object) {
			if (&object != this) {
				_dec_ref_count();
				_shallow_clean();
				_init(object._get_refer());
				__width = object.__width;
				__height = object.__height;
				__shareable = object.__shareable;
				__code_format = object.__code_format;
#pragma unroll
				for (size_t i = 0; i < 4; ++i) {
					__data[i] = object.__data[i];
					__pitch[i] = object.__pitch[i];
				}
				object._shallow_clean();
			}
			return *this;
		}
		MatData& operator= (const MatData& object) {
			if (&object != this) {
				_dec_ref_count();
				// why ?
				_shallow_clean();
				_init(object._get_refer());
				_add_ref_count();
				__width = object.__width;
				__height = object.__height;
				__shareable = object.__shareable;
				__code_format = object.__code_format;
#pragma unroll
				for (size_t i = 0; i < 4; ++i) {
					__data[i] = object.__data[i];
					__pitch[i] = object.__pitch[i];
				}
			}
			return *this;
		}
	public:
		MatData crop(int left, int right, int top, int bottom) {
			return rect(left, right, top, bottom);
		}
		MatData rect(int left, int right, int top, int bottom) {
			MatData region(false);
			region.__width = right - left;
			region.__height = bottom - top;
			region.__code_format = __code_format;
			// to be modification
			region.__pitch[0] = __pitch[0];
			region.__data[0] = &(__data[0][top * __pitch[0] + left * __parse_format_code<base::image_info::element_number>()]);
			if(base::any_equel(__code_format, 131328, 131329)) {
				region.__pitch[1] = __pitch[1];
				region.__data[1] = &(__data[1][(top >> 1) * __pitch[0] + ((left >> 1) << 1)]);
			}
			//for(int i = 0; i < __parse_format_code<base::image_info::plane_number>(); ++i) {
			//	region.__pitch[i] = __pitch[i];
			//	region.__data[i] = &(__data[i][top >> i * __pitch[i] + left * __parse_format_code<base::image_info::element_number>()]);
			//}
			return region;
		}
	public:
		class iterator {
		public:
			//using difference_type = typename _Myvec::difference_type;
			using pointer = _data_type*;
			using reference = _data_type&;
			using value_type = _data_type;
			using iterator_category = std::random_access_iterator_tag;
		};
	public:
		_data_type* operator[] (int index) {
			return __data[index];
		}
		_data_type* operator[] (int index) const {
			return __data[index];
		}
		template<typename _out_iterator>
		friend const _out_iterator& operator<< (_out_iterator& os, const MatData& mat) {
			for (int i = 0; i < mat.get_height(); ++i) {
				for (int j = 0; j < mat.get_width(); ++j) {
#pragma unroll
					for (int k = 0; k < mat.get_elements(); ++k) {
						os << std::right << std::setw(3) << int(mat[0][i * mat.get_pitch() + j * mat.get_elements() + k]) << "  ";
					}
				}
				os << std::endl;
			}
			return os;
		}
		//_data_type* operator[] (int row, int col) {
		//	return __data[row * __pitch[0] + col * __parse_format_code<base::image_info::element_number>()];
		//}
		//_data_type* operator[] (int row, int col) const {
		//	return __data[row * __pitch[0] + col * __parse_format_code<base::image_info::element_number>()];
		//}
	public:
		_data_type* data(int index = 0) {
			return __data[index];
		}
		_data_type* data(int index = 0) const {
			return __data[index];
		}
		MatData& copy() {
			//return MatData(*this);
			auto ret = MatData(__width, __height, __code_format);
			ret.__copy_data(*this);
			return ret;
		}
		void set_value(_data_type value) {
			// assert(__code_format != base::image_format::image_format_nv12 || __code_format != base::image_format::image_format_nv21);
			// it is correct only the data type is single type
			// if the data type is int, then the initial value
			// is 0x(value)(value)(value)(value)
			// data is or not continues in the rows's point of view
			if (__pitch[0] == __cacu_pitch(__width, 8 * __parse_format_code<base::image_info::element_number>())) {
				std::memset(__data[0], value, __chunck_size[0]);
			}
			else {
				for (int i = 0; i < __height; ++i) {
					std::memset(&__data[0][i * __pitch[0]], value, __width * __parse_format_code<base::image_info::element_number>());
				}
			}
		}
	public:
		std::string get_info(std::string name = "dummy") {
			std::stringstream info;
			info << name << " -- " << "width : " << __width << ", height : " << __height << ", format : " << __code_format << std::endl;
			return info.str();
		}
	public:
		const int get_width() const {
			return __width;
		}
		const int get_height() const {
			return __height;
		}
		const int get_pitch(int query = 0) const {
			return __pitch[query];
		}
		const int get_elements() const {
			return __parse_format_code<base::image_info::element_number>();
		}
		const int get_format_code() const {
			return __code_format;
		}
		// when the data format is nv12 or nv21, be careful
		_data_type* get_data(int plane_index = 0, int row = 0) {
			return &__data[plane_index][row * __pitch[plane_index]];
		}
		const _data_type* get_data(int plane_index = 0, int row = 0) const {
			return &__data[plane_index][row * __pitch[plane_index]];
		}
	public:
		// no need
		//base::return_code allocator(int width, int height, int code_format) {
		//	__width = width;
		//	__height = height;
		//	__code_format = code_format;
		//	__allocator();
		//	if (nullptr != __data[0]) {
		//		return base::return_code::success;
		//	}
		//	else {
		//		return base::return_code::out_of_memory;
		//	}
		//}
	private:
		void __copy_data(const MatData& object) {
			if (__pitch[0] == __cacu_pitch(__width, 8 * __parse_format_code<base::image_info::element_number>())) {
				// continues
				for (size_t i = 0; i < __parse_format_code<base::image_info::plane_number>(); ++i) {
					std::copy_n(object.__data[i], __chunck_size[i], __data[i]);
				}
			}
			else {
				// not continues(rect  region)
				for (size_t i = 0; i < __parse_format_code<base::image_info::plane_number>(); ++i) {
					for (int i = 0; i < __height; ++i) {
						std::copy_n(&object.__data[i][i * __pitch[i]], __pitch[i], __data[i]);
					}
				}
			}
		}
	private:
		void __shallow_clean() {
			__width = 0;
			__height = 0;
			__code_format = 0;
			for (size_t i = 0; i < 4; ++i) {
				__pitch[i] = 0;
				__data[i] = nullptr;
				__chunck_size[i] = 0;
			}
		}
		void __allocator() {
			__get_format_details();
			for (size_t i = 0; i < __parse_format_code<base::image_info::plane_number>(); ++i) {
				__data[i] = new _data_type[__chunck_size[i]]{ 0 };
			}
		}
		void __dellocator() {
			for (size_t i = 0; i < 4; ++i) {
				delete[] __data[i];
			}
		}
	private:
		template<base::image_info _query>
		int __parse_format_code() const {
			return (__code_format >> (8 * (int(_query) - 1))) & 0x00ff;
		}
		int __cacu_pitch(int width, int bit_count) {
			return (((int)(width) * (bit_count)+31) / 32 * 4);
		}
		void __cacu_chunck_size() {
			for (size_t i = 0; i < 4; ++i) {
				__chunck_size[i] = __height * __pitch[i];
			}
		}
		void __adjust_chunck_size() {
			if (base::any_equel(__code_format, 131328, 131329)) {
				__chunck_size[1] = __height * __pitch[1] / 2;
			}
		}
		void __get_format_details() {
			for (size_t i = 0; i < __parse_format_code<base::image_info::plane_number>(); ++i) {
				__pitch[i] = __cacu_pitch(__width, 8 * __parse_format_code<base::image_info::element_number>());
			}
			__cacu_chunck_size();
			__adjust_chunck_size();
		}
	private:
		bool __shareable;
	private:
		int __width;
		int __height;
		int __pitch[4];
		int __code_format;
		int __chunck_size[4];
	private:
		alignas(_align_size) _data_type* __data[4];
	private:
		friend ReferCount<MatData>;
	};

	template<typename _data_type, int _align_size>
	class Tensor;

	template<class _derived>
	class TensorRefer {
	protected:
		int* _get_refer() {
			return __refer_count;
		}
		void _shallow_clean() {
			__release();
		}
		void _add_ref_count() {
			if (__refer_count) {
				(*__refer_count)++;
			}
		}
		void _dec_ref_count() {
			if (__refer_count) {
				if (((*__refer_count)--) == 1) {
					__uninit();
				}
				__release();
			}
		}
		void _init(int* ref_count) {
			__refer_count = ref_count;
		}
	private:
		_derived& __derived() { 
			return *static_cast<_derived*>(this); 
		}
		const _derived& __derived() const {
			return *static_cast<const _derived*>(this); 
		}
		void __release() {
			__refer_count = nullptr;
			__derived().__shallow_clean();
		}
		void __uninit() {
			delete __refer_count;
			__derived().__dellocator();
			//__derived().__align_dellocator();
		}
	private:
		int* __refer_count;
	};

	template<typename _data_type = unsigned char, int _align_size = 64>
	class Tensor : public TensorRefer<Tensor<_data_type, _align_size>> {
	public:
		Tensor() {
			_shallow_clean();
			_init(new int(1));
		}
		Tensor(int cols, int rows, int channels) : __cols(cols), __rows(rows), __channels(channels), __data(nullptr) {
			_init(new int(1));
			__allocator();
			//__align_allocator();
		}
		~Tensor() {
			_dec_ref_count();
		}
	public:
		Tensor(Tensor&& object) {
			_shallow_clean();
			_init(object._get_refer());
			__cols = object.__cols;
			__rows = object.__rows;
			__data = object.__data;
			__channels = object.__channels;
			object._shallow_clean();
		}
		Tensor(const Tensor& object) {
			_shallow_clean();
			_init(object._get_refer());
			_add_ref_count();
			__cols = object.__cols;
			__rows = object.__rows;
			__data = object.__data;
			__channels = object.__channels;
		}
	public:
		Tensor& operator= (Tensor&& object) {
			if (this != &object) {
				_dec_ref_count();
				_shallow_clean();
				_init(object._get_refer());
				__cols = object.__cols;
				__rows = object.__rows;
				__data = object.__data;
				__channels = object.__channels;
				object._shallow_clean();
			}
			return *this;
		}
		Tensor& operator= (const Tensor& object) {
			if (this != &object) {
				_dec_ref_count();
				_shallow_clean();
				_init(object._get_refer());
				_add_ref_count();
				__cols = object.__cols;
				__rows = object.__rows;
				__data = object.__data;
				__channels = object.__channels;
			}
			return *this;
		}
	public:
		int cols() const {
			return __cols;
		}
		int rows() const {
			return __rows;
		}
		int channels() const {
			return __channels;
		}
	public:
		_data_type* operator[] (int index) {
			return __data[index];
		}
		_data_type* operator[] (int index) const {
			return __data[index];
		}
		_data_type* get_data(int cols, int rows) {
			return __data + rows * __cols * __channels + cols * __channels;
		}
		_data_type* get_data(int cols, int rows) const {
			return __data + rows * __cols * __channels + cols * __channels;
		}
		template<typename _out_iterator>
		friend const _out_iterator& operator<< (_out_iterator& os, const Tensor& tensor) {
			for (int i = 0; i < tensor.rows(); ++i) {
				for (int j = 0; j < tensor.cols(); ++j) {
#pragma unroll
					for (int k = 0; k < tensor.channels(); ++k) {
						os << std::right << std::setw(3) << int(tensor.get_data(i, j)[k]) << "  ";
					}
				}
				os << std::endl;
			}
			return os;
		}
	private:
		void __shallow_clean() {
			__cols = 0;
			__rows = 0;
			__channels = 0;
			__data = nullptr;
		}
		void __allocator() {
			__data = new _data_type[__cols * __rows * __channels * sizeof(_data_type)]{ 0 };
		}
		void __dellocator() {
			delete[] __data;
		}
		//实际在计算机系统中，数据在内存中都是以补码的形式进行存储的
		//当n等于16时，其-n即为-16， -16的原码为1000...0010000, 其反码为1111...1101111,
		//补码为1111...1110000，相当于取的是地址的高位，低位直接截断进行对齐
		template<typename data_type>
		data_type* __align_pointer(data_type* ptr, int align_size = _align_size) {
			return (data_type*)(((size_t)ptr + align_size - 1) & -align_size);
		}
		void __align_allocator() {
			// sizeof(void*) is the u_ptr address size
			_data_type* u_ptr = (_data_type*)std::malloc(__cols * __rows * __channels * sizeof(_data_type) + sizeof(void*) + _align_size);
			_data_type** a_ptr = __align_pointer<_data_type*>((_data_type**)u_ptr + 1, _align_size);
			a_ptr[-1] = u_ptr;
			__data = (_data_type*)a_ptr;
		}
		void __align_dellocator() {
			auto u_ptr = ((_data_type**)__data)[-1];
			std::free(u_ptr);
		}
	private:
		int __cols;
		int __rows;
		int __channels;
		alignas(_align_size) _data_type* __data;
	private:
		friend TensorRefer<Tensor>;
	};

	namespace colorspace {
		using data_type = unsigned char;
		using Mat = MatData<unsigned char, 64>;

		template<typename _args, typename _func>
		auto operator| (_args&& args, const _func& func) -> decltype(func(std::forward<_args>(args))) {
			if (true == args) {
				return func(std::forward<_args>(args));
			}
		}

		inline void BGR2YUV(data_type b, data_type g, data_type r, data_type& y, data_type& u, data_type& v) {
			// not implemented
			unsigned int y1 = (unsigned int)(y * 0x0101 * 18997) >> 16;
			y = std::clamp((int)((-(u * UB) + y1 + BB) >> 6), 0, 255);
			u = std::clamp((int)(-(v * VG + u * UG) + y1 + BG) >> 6, 0, 255);
			v = std::clamp((int)(-(v * VR) + y1 + BR) >> 6, 0, 255);
		}
		inline void YUV2BGR(data_type y, data_type u, data_type v, data_type& b, data_type& g, data_type& r) {
			unsigned int y1 = (unsigned int)(y * 0x0101 * 18997) >> 16;
			b = std::clamp((int)((-(u * UB) + y1 + BB) >> 6), 0, 255);
			g = std::clamp((int)(-(v * VG + u * UG) + y1 + BG) >> 6, 0, 255);
			r = std::clamp((int)(-(v * VR) + y1 + BR) >> 6, 0, 255);
		}

		void color_convert_bgr_2_yuv_row(data_type* in, data_type* out, int width) {
			for (int i = 0; i < width; ++i) {
				YUV2BGR(*in++, *in++, *in++, *out++, *out++, *out++);
			}
		}
		void color_convert_yuv_2_bgr_row(data_type* in, data_type* out, int width) {
			for (int i = 0; i < width; ++i) {
				YUV2BGR(*in++, *in++, *in++, *out++, *out++, *out++);
			}
		}
		template<int _in_code_format, int _out_code_format>
		base::return_code color_convert_impl(Mat& in, Mat& out) {
			return base::return_code::unsupport;
		}
		template<>
		base::return_code color_convert_impl<66304, 66306>(Mat& in, Mat& out) {
			for (size_t i = 0; i < in.get_height(); ++i) {
				auto in_data = in.get_data(0, i);
				auto out_data = out.get_data(0, i);
				color_convert_bgr_2_yuv_row(in_data, out_data, in.get_width());
			}
			return base::return_code::success;
		}
		template<>
		base::return_code color_convert_impl<66306, 66304>(Mat& in, Mat& out) {
			for (size_t i = 0; i < in.get_height(); ++i) {
				auto in_data = in.get_data(0, i);
				auto out_data = out.get_data(0, i);
				color_convert_yuv_2_bgr_row(in_data, out_data, in.get_width());
			}
			return base::return_code::success;
		}
		base::return_code color_convert(Mat& in, Mat& out) {
			// assert size
			// to do ...
			(in.get_format_code() == 66304) | [&](bool) { color_convert_impl<66304, 66306>(in, out); };
			(in.get_format_code() == 66306) | [&](bool) { color_convert_impl<66306, 66304>(in, out); };
			return 	base::return_code::success;
		}
	}
}
