#pragma once

#include <deque>
#include <regex>
#include <mutex>
#include <queue>
#include <atomic>
#include <future>
#include <thread>
#include <vector>
#include <numeric>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <functional>
#include <condition_variable>

#include <assert.h>

namespace pattern{

	template<typename _derived>
	class Iterator : public std::iterator<std::forward_iterator_tag, _derived> {
	protected:
		Iterator<_derived>& range;                       // Reference to the range for this iterator
		_derived value;                                       // Value pointed to
	public:
		explicit Iterator(Iterator<_derived>& a_range) : range{ a_range }, value{ a_range.start } {};
		// Assignment operator
		Iterator& operator=(const Iterator& src) {
			range = src.range;
			value = src.value;
		}
		// Dereference an iterator
		_derived& operator*() {
			// When the value is one step more than the last, it's an end iterator
			if (value == static_cast<_derived>(range.start + range.count*range.step)) {
				throw std::logic_error("Cannot dereference an end iterator.");
			}
			return value;
		}
		// Prefix increment operator
		Iterator& operator++() {
			// When the value is one step more than the last, it's an end iterator
			if (value == static_cast<_derived>(range.start + range.count*range.step)) {
				throw std::logic_error("Cannot increment an end iterator.");
			}
			value += range.step;                         // Increment the value by the range step
			return *this;
		}
		// Postfix increment operator
		Iterator operator++(int) {
			// When the value is one step more than the last, it's an end iterator
			if (value == static_cast<_derived>(range.start + range.count*range.step)) {
				throw std::logic_error("Cannot increment an end iterator.");
			}
			auto temp = *this;
			value += range.step;                         // Increment the value by the range step
			return temp;                                 // _derivedhe iterator before it's incremented
		}
		// Comparisons
		bool operator<(const Iterator& iter) const { return value < iter.value; }
		bool operator==(const Iterator& iter) const { return value == iter.value; }
		bool operator!=(const Iterator& iter) const { return value != iter.value; }
		bool operator>(const Iterator& iter) const { return value > iter.value; }
		bool operator<=(const Iterator& iter) const { *this < iter || *this == iter; }
		bool operator>=(const Iterator& iter) const { *this > iter || *this == iter; }
	private:
		friend class Iterator<_derived>;
	};
}