#pragma once

#include <deque>
#include <queue>
#include <mutex>
#include <future>
#include <vector>
#include <thread>
#include <atomic>
#include <numeric>
#include <iostream>
#include <functional>
#include <condition_variable>

#include <module_ai_pointer.h>
#include <module_refer_count.h>

namespace module{

	template<typename _object>
	class AiPointer : public ReferCount<AiPointer<_object>> {
	public:
		AiPointer() : __object(nullptr) {
			__init_new();
		}
		~AiPointer() {
			__dec_ref_count();
		}
	public:
		AiPointer(_object * _pointer) {
			__init_new();
			__object = _pointer;
		}
		AiPointer(_object * _point, int * _refer_count) {
			__object = _point;
			__refer_count = _refer_count;
		}
	public:
		template<typename _anther_object>
		AiPointer(AiPointer<_anther_object>&& _anther_ai_point) {
			__object = _anther_ai_point.__object;
			__refer_count = _anther_ai_point.__refer_count;
			_anther_ai_point.__release();
			_anther_ai_point.__release_refer_count();
		}

		template<typename _anther_object>
		AiPointer(const AiPointer<_anther_object>& _anther_ai_point) {
			_anther_ai_point.__add_ref_count();
			__object = _anther_ai_point.__object;
			__refer_count = _anther_ai_point.__refer_count;
		}

		template<typename _anther_object>
		AiPointer& operator= (AiPointer<_anther_object>&& _anther_ai_point) {
			if (this != static_cast<const void*>(&_anther_ai_point)) {
				__dec_ref_count();
				__refer_count = _anther_ai_point.__refer_count;
				__object = static_cast<_object*>(_anther_ai_point.__object);
				_anther_ai_point.__release();
				_anther_ai_point.__release_refer_count();
			}
			return *this;
		}

		template<typename _anther_object>
		AiPointer& operator= (const AiPointer<_anther_object>& _anther_ai_point) {
			if (this != static_cast<const void*>(&_anther_ai_point)) {
				__dec_ref_count();
				_anther_ai_point.__add_ref_count();
				__refer_count = _anther_ai_point.__refer_count;
				__object = static_cast<_object*>(_anther_ai_point.__object);
			}
			return *this;
		}

	public:
		bool is_empty() const {
			return (nullptr == __object);
		}
		explicit operator bool() const {
			return (nullptr != __object);
		}
		template<typename _anther_object>
		AiPointer<_anther_object> cast() {
			if (is_empty()) {
				return AiPointer<_anther_object>();
			} else {
				__add_ref_count();
				return AiPointer<_anther_object>(static_cast<_anther_object*>(__object), __refer_count);
			}
		}
		template<typename _anther_object>
		const AiPointer<_anther_object> cast() const {
			if (is_empty()) {
				return AiPointer<_anther_object>();
			} else {
				__add_ref_count();
				return AiPointer<_anther_object>(static_cast<_anther_object*>(__object), __refer_count);
			}
		}

		_object* get() const {
			return __object;
		}
		_object& operator*() const {
			return *__object;
		}
		_object* operator->() const {
			return __object;
		}

		bool operator== (const AiPointer& _anther_ai_point) const {
			return (__object == _anther_ai_point.__object);
		}
		// why not use the same operation ?
		bool operator!= (const AiPointer& _anther_ai_point) const {
			return !(__object == _anther_ai_point.__object);
		}

	private:
		void __release() {
			__object = nullptr;
		}
		void __deallocate() {
			delete __object;
		}
	private:
		void __release_refer_count() {
			__refer_count = nullptr;
		}
	private:
		_object *__object;
	private:
		friend ReferCount<AiPointer<_object>>;
	};
	template<typename _typename, typename... _args>
	inline AiPointer<_typename> make_ai_pointer(_args&&... args) {
		return AiPointer<_typename>(new _typename(std::forward<_args>(args)...));
	}
}
