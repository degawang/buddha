#pragma once

#include <deque>
#include <queue>
#include <mutex>
#include <chrono>
#include <future>
#include <vector>
#include <thread>
#include <atomic>
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

	template<class _derived, bool _shareable = true>
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
		void __add_ref_count() {
			//std::lock_guard<std::recursive_mutex> lock(__mutex);
			if (__refer_count) {
				(*__refer_count)++;
			}
		}
		void __dec_ref_count() {
			//std::lock_guard<std::recursive_mutex> lock(__mutex);
			if (__refer_count) {
				if (((*__refer_count)--) == 1) {
					__uninit();
					__release();
				}
			}
		}
		_derived& __derived() { 
			return *static_cast<_derived*>(this); 
		}
		const _derived& __derived() const {
			return *static_cast<const _derived*>(this); 
		}
		void __release() {
			__refer_count = nullptr;
			__derived().__clear();
		}
		void __init_new() {
			__refer_count = new int(1);
		}
		void __init_with(int count, int* ref_count) {
			*ref_count = count;
			__refer_count = ref_count;
		}
		void __uninit() {
			delete __refer_count;
			__derived().__dellocator();
		}
	protected:
		bool __shareable;
		int* __refer_count;
		//std::recursive_mutex __mutex;
	};

	template<typename _data_type = unsigned char, int _align_size = 64>
	class MatData : public ReferCount<MatData<_data_type, _align_size>, true> {
	public:
		MatData() {
			__clear();
			__init_new();
		}
		MatData(int width, int height, int code_format)
			: __width(width), __height(height), __pitch{ 0 }, __channels(0), __chunck_size{ 0 }, __code_format(code_format), __data{ nullptr } {
			__clear();
			__init_new();
			__allocator();
		}
		~MatData() {
			__dec_ref_count();
		}
	public:
		const int get_width() const {
			return __width;
		}
		const int get_height() const {
			return __height;
		}
		int get_format_code() const {
			return int(__code_format);
		}
		// when the data format is nv12 or nv21, be careful
		_data_type* get_data(int plane_index = 0, int row = 0) {
			return &__data[plane_index][row * __pitch[plane_index]];
		}
		_data_type* get_data(int plane_index = 0, int row = 0) const {
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
		void __clear() {
			__width = 0;
			__height = 0;
			__channels = 0;
			__code_format = 0;
			for (size_t i = 0; i < 4; ++i) {
				__pitch[i] = 0;
				__data[i] = nullptr;
				__chunck_size[i] = 0;
			}
		}
		void __allocator() {
			__get_format_details();
			for (size_t i = 0; i < 4; ++i) {
				__data[i] = new _data_type[__chunck_size[i]];
			}
		}
		void __dellocator() {
			for (size_t i = 0; i < 4; ++i) {
				delete[] __data[i];
			}
		}
	private:
		template<int _query>
		int __parse_format_code() {
			return (__code_format >> (8 * (_query - 1))) & 0x00ff;
		}
		int __cacu_pitch(int width, int bit_count) {
			return (((int)(width) * (bit_count)+31) / 32 * 4);
		}
		void __cacu_memory_size() {
			for (size_t i = 0; i < 4; ++i) {
				__chunck_size[i] = __height * pitch[i];
			}
		}
		void __adjust_memory_size() {
			switch (__code_format) {
			case base::format_code<2,1,0>:
			case base::format_code<2,1,1>:
				__height * __pitch[1] / 2;
			}
		}
		void __get_format_details() {
			for (size_t i = 0; i < __parse_format_code<3>(); ++i) {
				__pitch[i] = __cacu_pitch(__width, 8 * __parse_format_code<2>());
			}
			__cacu_memory_size();
			__adjust_memory_size();
		}
	private:
		int __width;
		int __height;
		int __pitch[4];
		int __channels;
		int __code_format;
		int __chunck_size[4];
	private:
		alignas(_align_size) _data_type* __data[4];
	private:
		friend ReferCount<MatData>;
	};

	namespace colorspace {
		using data_type = unsigned char;
		using Mat = MatData<unsigned char, 64>;

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
		base::return_code color_convert_impl<int(base::image_format::image_format_bgr), int(base::image_format::image_format_yuv)>(Mat& in, Mat& out) {
			for (size_t i = 0; i < in.get_height(); ++i) {
				auto in_data = in.get_data(0, i);
				auto out_data = out.get_data(0, i);
				color_convert_bgr_2_yuv_row(in_data, out_data, in.get_width());
			}
			return base::return_code::success;
		}
		template<>
		base::return_code color_convert_impl<int(base::image_format::image_format_yuv), int(base::image_format::image_format_bgr)>(Mat& in, Mat& out) {
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
			return color_convert_impl<8, 8>(in, out);
		}
	}
}
