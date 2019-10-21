//
// Created by Janos Meny on 9/4/19.
//

#pragma once

#include <fmt/core.h>

#include <chrono>
#include <string>
#include <unordered_map>
#include <mutex>




template<class Ratio>
std::string toSI()
{
    if constexpr(std::is_same_v<std::nano, Ratio>)
        return "nano seconds";
    if constexpr(std::is_same_v<std::micro, Ratio>)
        return "micro seconds";
    if constexpr(std::is_same_v<std::milli, Ratio>)
        return "milli seconds";
    if constexpr(std::is_same_v<std::centi, Ratio>)
        return "seconds";
    if constexpr(std::is_same_v<std::ratio<60>, Ratio>)
        return "minutes";
    if constexpr(std::is_same_v<std::ratio<3600>, Ratio>)
        return "hours";
}


class ScopedTimer
{
public:
    using my_clock = std::chrono::steady_clock;
    using my_duration = std::chrono::nanoseconds;
    using time_point_t = std::chrono::time_point<my_clock>;

    explicit ScopedTimer(std::string name): name_(std::move(name)), start_(my_clock::now()){}

    ~ScopedTimer()
    {
        const auto end = my_clock::now();
        my_duration time = end - start_;
        std::lock_guard l(mutex_);
        auto& [duration, count] = log_[name_];
        duration += time;
        ++count;
    }

    template<class Ratio = std::centi>
    static void printStatistics()
    {
        using user_dur = std::chrono::duration<long double, Ratio>;
        std::lock_guard l(mutex_);
        for(const auto& [name, durAndCounter]: log_)
        {
            const auto& [duration, count] = durAndCounter;
            user_dur average = duration / count;
            fmt::print("Average time of {} was {} {}\n", name, average.count(), toSI<Ratio>());
        }
    }

private:
    struct DurationAndCounter
    {
        my_duration duration{0};
        std::size_t count = 0;
    };

    static std::unordered_map<std::string, DurationAndCounter> log_;
    static std::mutex mutex_;

    std::string name_;
    time_point_t start_;
};
