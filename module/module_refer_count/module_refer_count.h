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

#include <assert.h>

namespace module {

	template<class _derived>
	class ReferCount {
	public:
		ReferCount() : __refer_count(nullptr) {
		}

		ReferCount(int* _ref_count) : __refer_count(_ref_count) {
		}

		virtual ~ReferCount() {
		}

		ReferCount(const ReferCount& _anther) : __refer_count(_anther.__refer_count) {
			__add_ref_count();
		}

		ReferCount(ReferCount&& _anther) : __refer_count(_anther.__refer_count) {
			_anther.__refer_count = nullptr;
		}

		ReferCount& operator = (const ReferCount& _anther) {
			if (&_anther != this) {
				__dec_ref_count();
				_anther.__add_ref_count();
				__refer_count = _anther.__refer_count;
			}
			return *this;
		}

		ReferCount& operator = (ReferCount&& _anther) {
			if (&_anther != this) {
				__dec_ref_count();
				__refer_count = _anther.__refer_count;
				_anther.__refer_count = nullptr;
			}
			return *this;
		}

		bool operator == (const ReferCount& _anther) const {
			return __refer_count == _anther.__refer_count;
		}

		bool operator != (const ReferCount& _anther) const {
			return __refer_count != _anther.__refer_count;
		}
	protected:
		void __add_ref_count() {
			std::lock_guard<std::recursive_mutex> lock(__mutex);
			if (__refer_count) {
				(*__refer_count)++;
			}
		}

		void __dec_ref_count() {
			std::lock_guard<std::recursive_mutex> lock(__mutex);
			if (__refer_count) {
				if (((*__refer_count)--) == 1) {
					__deallocate();
					__release();
				}
			}
		}

		int __get_ref_count() const { 
			return ((__refer_count != nullptr) ? (*__refer_count) : 0);
		}

		bool __is_no_ref() const { 
			return (__refer_count == nullptr);
		}

		_derived& __derived() { 
			return *static_cast<_derived*>(this); 
		}
		const _derived& __derived() const {
			return *static_cast<const _derived*>(this); 
		}

		void __release() {
			std::lock_guard<std::recursive_mutex> lock(__mutex);
			__refer_count = nullptr;
			__derived().__release();
		}

		void __allocate() {
			__refer_count = new int(1);
		}

		void __init_new() {
			__refer_count = new int(1);
		}

		void __init_with(int _count, int* _ref_count) {
			*_ref_count = _count;
			__refer_count = _ref_count;
		}

		void __deallocate() {
			std::lock_guard<std::recursive_mutex> lock(__mutex);
			delete __refer_count;
			__derived().__deallocate();
		}

	protected:
		int* __refer_count;
		std::recursive_mutex __mutex;
	};
}