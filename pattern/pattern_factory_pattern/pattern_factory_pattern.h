#pragma once

#include <map>
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
#include <filesystem>
#include <functional>
#include <condition_variable>

#include <assert.h>

namespace pattern{
	// still can not split the customer and the dispatcher
	class BaseOperation {
	public:
		BaseOperation() = default;
		virtual ~BaseOperation() {}
	public:
		template<typename ..._args>
		BaseOperation(_args ...args){}
	public:
		void __same_operation() {}
		virtual void __different_operation() {}
	};
	class FactoryPattern {
	public:
		FactoryPattern() = default;
		virtual ~FactoryPattern() {}
	public:
		virtual std::shared_ptr<BaseOperation> get_instance() = 0;
	private:
		//std::map<module::image_format, std::function<BaseFactory*()>> __map;
	};

	class ConcreteOperation : public BaseOperation {
	public:
		ConcreteOperation() = default;
		virtual ~ConcreteOperation() {}
	public:
		virtual void __different_operation() {}
	};

	class ConcreteFactory : public FactoryPattern {
	public:
		ConcreteFactory() = default;
		virtual ~ConcreteFactory() {}
	public:
		template<typename ..._args>
		std::shared_ptr<BaseOperation> get_instance(_args... args) {
			return std::make_shared<ConcreteOperation>(std::forward<_args>(args)...);
		}
	};
}