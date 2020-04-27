#pragma once

#include <map>
#include <deque>
#include <queue>
#include <mutex>
#include <chrono>
#include <future>
#include <vector>
#include <thread>
#include <atomic>
#include <fstream>
#include <numeric>
#include <iostream>
#include <functional>
#include <condition_variable>

#include "../../base/base.h"
#include "algorithm_base.h"
#include "module_time_clock.h"

namespace module {
	
#define RECORD_NAME "algorithm.txt"

#define STREAM_TYPE Dummy
//#define STREAM_TYPE std::ostream
//#define STREAM_TYPE std::ofstream

	struct Dummy {
		using type = Dummy;
		template<typename ..._args>
		Dummy(_args&&... args) {}
		template<typename ..._args>
		Dummy& operator<< (_args&&... args) {
			return *this;
		}
		template<typename ..._args>
		Dummy& operator>> (_args&&... args) {
			return *this;
		}
	};
	class InforWrap : public pattern::SingletonPattern<InforWrap> {
	public:
		~InforWrap() {
			__iof_stream.close();
		}
	public:
		std::ofstream& get_stream() {
			return __iof_stream;
		}
	private:
		InforWrap(std::string iof_path) :  __iof_path(iof_path), __iof_stream() {
			if (std::filesystem::exists(std::filesystem::path(__iof_path))) {
				std::filesystem::remove(__iof_path);
			}
			__iof_stream.open(__iof_path, std::ios::app);
		}
	private:
		std::string __iof_path;
		std::ofstream __iof_stream;
	private:
		friend pattern::SingletonPattern<InforWrap>;
	};
	template<base::infor_type _info_type>
	struct InforLevel;
	template<>
	struct InforLevel<base::infor_type::dummy> {
		Dummy& get_info_type() {
			return Dummy();
		}
	};
	template<>
	struct InforLevel<base::infor_type::console> {
		std::ostream& get_info_type() {
			return std::cout;
		}
	};
	template<>
	struct InforLevel<base::infor_type::std_file> {
		std::ofstream& get_info_type() {
			return InforWrap::get_instance(RECORD_NAME)->get_stream();
		}
	};
	template<typename _stream_type>
	class InforAdaptor : public pattern::SingletonPattern<InforAdaptor<_stream_type>> {
	public:
		~InforAdaptor() = default;
	private:
		InforAdaptor(_stream_type& stream_object) : __stream_object(stream_object) {
		}
	public:
		template<typename ..._args>
		void in_formation(_args&&... args) {
			std::lock_guard<std::mutex> lock(__mutex);
			__in_stream(std::forward<_args>(args)...);
		}
	private:
		void __in_stream() {};
		template<typename _head, typename ..._rest>
		void __in_stream(_head&& head, _rest&&... rest) {
			__stream_object >> head;
			__in_stream(std::forward<_rest>(rest)...);
		}
	public:
		template<typename ..._args>
		void out_formation(_args&&... args) {
			std::lock_guard<std::mutex> lock(__mutex);
			__stream_object << "debug information : ";
			__out_stream(std::forward<_args>(args)...);
			// self defined << can not be accepted by the std::endl function
			//__stream_object << std::endl;
			__stream_object << "\n";
		}
	private:
		void __out_stream() {};
		template<typename _head,typename ..._rest>
		void __out_stream(_head&& head, _rest&&... rest) {
			__stream_object << head << "  ";
			__out_stream(std::forward<_rest>(rest)...);
		}
	private:
		std::mutex __mutex;
		_stream_type& __stream_object;
	private:
		friend pattern::SingletonPattern<InforAdaptor>;
	};
	template<typename _stream_type = STREAM_TYPE>
	class Information {
	public:
		template<typename ..._args>
		Information(std::string function_name, _args&&... args) : __function_name(function_name) {
			auto infor_adaptor = InforAdaptor<_stream_type>::get_instance(ObjectAdaptor<_stream_type>().get_object());
			infor_adaptor->out_formation(std::forward<_args>(args)...);
		}
	public:
		~Information() {
			auto infor_adaptor = InforAdaptor<_stream_type>::get_instance(ObjectAdaptor<_stream_type>().get_object());
			infor_adaptor->out_formation(__function_name.c_str(), " cost ", __time_clock.time_duration(), " ms");
		}
	private:
		//template<typename _type>
		//typename std::enable_if<std::is_same<_stream_type, Dummy>::value, Dummy>::type& __get_object() {
		//	return Dummy();
		//}
		//template<typename _type>
		//typename std::enable_if<std::is_same<_stream_type, std::ostream>::value, std::ostream>::type& __get_object() {
		//	return std::cout;
		//}
		//template<typename _type>
		//typename std::enable_if<std::is_same<_stream_type, std::ofstream>::value, std::ofstream>::type& __get_object() {
		//	return InforWrap::get_instance(RECORD_NAME)->get_stream();
		//}
		//template<>
		//typename std::conditional<std::is_same<_stream_type, Dummy>::value, Dummy, Dummy>::type& __get_object() {
		//	return Dummy();
		//}
	private:
		// original class type is not defined yet, so it is impossible to get a <> type object
		template<typename _dummy_type>
		struct ObjectAdaptor;
		template<>
		struct ObjectAdaptor<Dummy> {
			Dummy& get_object() {
				return Dummy();
			};
		};
		template<>
		struct ObjectAdaptor<std::ostream> {
			std::ostream& get_object() {
				return std::cout;
			}
		};
		template<>
		struct ObjectAdaptor<std::ofstream> {
			std::ofstream& get_object() {
				return InforWrap::get_instance(RECORD_NAME)->get_stream();
			}
		};
		//template<>
		//struct ObjectWrapper<std::ofstream> {
		//	typename std::conditional<std::is_same<_stream_type, Dummy>::value, Dummy, Dummy>::type& get_object() {
		//		return Dummy();
		//	}
		//};
	private:
		TimeClock __time_clock;
		std::string __function_name;
	};
}
