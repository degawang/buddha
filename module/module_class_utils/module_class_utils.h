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
}