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
#include <filesystem>
#include <functional>
#include <condition_variable>

namespace module {
    class NonCopyable {
    private:
        NonCopyable(NonCopyable const&) = delete;
        NonCopyable& operator= (NonCopyable const&) = delete;
    protected:
        NonCopyable() {};
        virtual ~NonCopyable() {};
    };

    template<class _type>
    class NonInheritable {
        friend class _type;
        NonInheritable() {};
    };
    class OldNonInherit : public NonInheritable<OldNonInherit> {
    };
    class NewNonInherit final{
    };

}