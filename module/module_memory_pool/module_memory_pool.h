#pragma once

/*-
 * Copyright (c) 2013 Cosku Acay, http://www.coskuacay.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/*-
folked from Acay, I did some modification:
1. derived from singleton pattern
2. change the naming style into mine
3. git url : https://github.com/cacay/MemoryPool
*/

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

#include "pattern_singleton_pattern.h"

namespace module {
	// singleton pattern can deal with the thread safe problem
	template <typename _type, size_t _block_size = 4096>
	class MemoryPool : public pattern::SingletonPattern<MemoryPool<_type, _block_size>> {
	public:
		typedef size_t				  size_type;
		typedef _type                 value_type;
		typedef _type*                pointer;
		typedef _type&                reference;
		typedef ptrdiff_t			  difference_type;
		typedef const _type*          const_pointer;
		typedef const _type&          const_reference;
		typedef std::true_type		  propagate_on_container_swap;
		typedef std::true_type		  propagate_on_container_move_assignment;
		typedef std::false_type		  propagate_on_container_copy_assignment;
	private:
		union slot {
			slot* __next;
			value_type __element;
		};
		typedef slot slot_type;
		typedef char* data_pointer;
		typedef slot* slot_pointer;
	private:
		MemoryPool() noexcept {
			__release();
		}
	public:
		MemoryPool(MemoryPool&& anther) noexcept {
			__current_block = anther.__current_block;
			anther.__current_block = nullptr;
			__free_slots = anther.__free_slots;
			__last_slots = anther.__last_slots;
			__current_slot = anther.__current_slot;
		}
		MemoryPool(const MemoryPool& anther) noexcept : MemoryPool() {}
		template <class _anther_type>
		MemoryPool(const MemoryPool<_anther_type>& anther) noexcept : MemoryPool() {}
	public:
		~MemoryPool() noexcept {
			auto current_block = __current_block;
			while (nullptr != current_block) {
				auto next_block = current_block->__next;
				operator delete(reinterpret_cast<void *>(current_block));
				current_block = next_block;
			}
		}
	public:
		MemoryPool &operator=(MemoryPool &&anther) noexcept {
			if (&anther != this) {
				__free_slots = anther.__free_slots;
				__last_slots = anther.__last_slot;
				__current_slot = anther.__current_slot;
				std::swap(__current_block, anther.__current_block);
			}
			return *this;
		}
		MemoryPool &operator=(const MemoryPool &anther) = delete;
	public:
		template <typename _anther_type>
		struct rebind { 
			typedef MemoryPool<_anther_type> other; 
		};
	public:
		template <typename... _args>
		_type* new_object(_args &&... args) {
			auto allocate_pointer = __allocate();
			new(allocate_pointer) _type(std::forward<_args>(args)...);
			return allocate_pointer;
		}
		void delete_object(_type *pointer) {
			if (nullptr != pointer) {
				pointer->~_type();
				__deallocate(pointer);
			}
		}
	private:
		size_type __pad_pointer(data_pointer pointer, size_type align_size) const noexcept {
			uintptr_t result = reinterpret_cast<uintptr_t>(pointer);
			return ((align_size - result) % align_size);
		}
	private:
		void __release() {
			__last_slot = nullptr;
			__free_slots = nullptr;
			__current_slot = nullptr;
			__current_block = nullptr;
		}
		pointer __address(reference element) const noexcept {
			return &element;
		}
		const_pointer __address(const_pointer element) const noexcept {
			return &element;
		}
		pointer __allocate(size_type count = 1, const_pointer hint = nullptr) {
			if (nullptr != __free_slots) {
				pointer result_address = reinterpret_cast<pointer>(__free_slots);
				__free_slots = __free_slots->__next;
				return result_address;
			} else {
				if (__current_slot >= __last_slot) {
					__allocate_block();
				}
				return reinterpret_cast<pointer>(__current_slot++);
			}
		}
		void __deallocate(pointer pointer, size_t hint = 1) {
			if (nullptr != pointer) {
				reinterpret_cast<slot_pointer>(pointer)->__next = __free_slots;
				__free_slots = reinterpret_cast<slot_pointer>(pointer);
			}
		}
		size_t __max_count() const noexcept {
			size_t max_blocks = -1 / _block_size;
			return (_block_size - sizeof(data_pointer)) / sizeof(slot_type) * max_blocks;
		}
	private:
		void __allocate_block() {
			data_pointer new_block = reinterpret_cast<data_pointer>(operator new(_block_size));
			reinterpret_cast<slot_pointer>(new_block)->__next = __current_block;
			__current_block = reinterpret_cast<slot_pointer>(new_block);
			data_pointer body = new_block + sizeof(slot_pointer);
			size_type body_padding = __pad_pointer(body, alignof(slot_type));
			__current_slot = reinterpret_cast<slot_pointer>(body + body_padding);
			__last_slot = reinterpret_cast<slot_pointer>(new_block + _block_size - sizeof(slot_type) + 1);
		}
		static_assert(_block_size >= 2 * sizeof(slot_type), "_block_size is too small.");
	private:
		slot_pointer __last_slot;
		slot_pointer __free_slots;
		slot_pointer __current_slot;
		slot_pointer __current_block;
	private:
		friend pattern::SingletonPattern<MemoryPool>;
	};
}
