#pragma once

#include <deque>
#include <queue>
#include <mutex>
#include <future>
#include <vector>
#include <thread>
#include <atomic>
#include <cstdint>
#include <numeric>
#include <utility> 
#include <iostream>
#include <functional>
#include <condition_variable>

#include <pattern_singleton_pattern.h>

namespace module{
	// is not thread safe
	// the block size should be 2^n
	template <typename _object, size_t _block_size = 4096>
	class MemoryPool : public pattern::SingletonPattern<MemoryPool<_object, _block_size>> {
	public:
		typedef size_t				  size_type;
		typedef _object               value_type;
		typedef _object*              pointer;
		typedef _object&              reference;
		typedef ptrdiff_t			  difference_type;
		typedef const _object*        const_pointer;
		typedef const _object&        const_reference;
		typedef std::true_type		  propagate_on_container_swap;
		typedef std::true_type		  propagate_on_container_move_assignment;
		typedef std::false_type		  propagate_on_container_copy_assignment;
	public:
		union slot;
	private:
		MemoryPool() noexcept {
			__release();
		}
		MemoryPool(const MemoryPool &_anther) noexcept : MemoryPool() {
		}
		MemoryPool(MemoryPool &&_anther) noexcept {
			__last_slot = _anther.__last_slot;
			__free_slots = _anther.__free_slots;
			__current_slot_ = _anther.__current_slot_;
			__current_block_ = _anther.__current_block_;
			_anther.__release();
		}
		template <class _anther_object>
		MemoryPool(const MemoryPool<_anther_object> &_anther) noexcept : MemoryPool() {
		}
	public:
		~MemoryPool() noexcept {
			slot *_current_block = __current_block;
			while (nullptr != _current_block) {
				slot *_next_block = _current_block->__next;
				delete(reinterpret_cast<void *>(_current_block));
				_current_block = _next_block;
			}
		}
	public:
		MemoryPool &operator=(MemoryPool &&_anther) noexcept {
			if (&_anther != this) {
				__last_slot_ = _anther.__last_slot_;
				__free_slots_ = _anther.__free_slots_;
				__current_slot = _anther.__current_slot;
				std::swap(__current_block_, _anther.__current_block_);
			}
			return *this;
		}
		MemoryPool &operator=(const MemoryPool &_anther) = delete;
	public:
		template <typename _anther_object>
		struct rebind { 
			typedef MemoryPool<_anther_object> other; 
		};
		pointer address(reference _element) const noexcept {
			return &_element;
		}
		const_pointer address(const_pointer _element) const noexcept {
			return &_element;
		}
		pointer allocate(size_type _count = 1, const_pointer _hint = nullptr) {
			if (nullptr != __free_slots) {
				pointer _result_address = reinterpret_cast<pointer>(__free_slots);
				__free_slots = __free_slots->__next;
				return _result_address;
			} else {
				if (__current_slot >= __last_slot) {
					__allocate_block();
				}
				return reinterpret_cast<pointer>(__current_slot++);
			}
		}
		void deallocate(pointer _pointer, size_t _hint = 1) {
			if (nullptr != _pointer) {
				reinterpret_cast<slot*>(_pointer)->__next = __free_slots;
				__free_slots = reinterpret_cast<slot*>(_pointer);
			}
		}
	public:
		template <typename... _args>
		_object* new_element(_args &&... args) {
			_object *_allocate_pointer = allocate();
			new (_allocate_pointer) _object(std::forward<_args>(args)...);
			return _allocate_pointer;
		}
		void delete_element(_object *_pointer) {
			if (nullptr != _pointer) {
				_pointer->~_object();
				deallocate(_pointer);
			}
		}
	private:
		size_type __pad_pointer(char * _pointer, size_type _align_size) const noexcept {
			uintptr_t _result = reinterpret_cast<uintptr_t>(_pointer);
			return ((_align_size - _result) % _align_size);
		}
	private:
	    size_t __max_count() const noexcept {
			size_t _max_blocks = -1 / _block_size;
			return (_block_size - sizeof(char *)) / sizeof(slot) * _max_blocks;
		}
		void __release() {
			__last_slot = nullptr;
			__free_slots = nullptr;
			__current_slot = nullptr;
			__current_block = nullptr;
		}
	private:
		void __allocate_block() {
			// allocate space for the new block and store a pointer to the previous one
			char* _new_block = reinterpret_cast<char*>(operator new(_block_size));
			reinterpret_cast<slot*>(_new_block)->__next = __current_block;
			__current_block = reinterpret_cast<slot*>(_new_block);
			// pad block body to staisfy the alignment requirements for elements
			char* _body = _new_block + sizeof(slot*);
			size_type _body_padding = __pad_pointer(_body, alignof(slot));
			__current_slot = reinterpret_cast<slot*>(_body + _body_padding);
			__last_slot = reinterpret_cast<slot*>(_new_block + _block_size - sizeof(slot) + 1);
		}
		static_assert(_block_size >= 2 * sizeof(slot), "_block_size is too small.");
	private:
		union slot {
			slot *__next;
			value_type __element;
		};
	private:
		slot *__last_slot;
		slot *__free_slots;
		slot *__current_slot;
		slot *__current_block;
	private:
		friend pattern::SingletonPattern<MemoryPool>;
	};
}
