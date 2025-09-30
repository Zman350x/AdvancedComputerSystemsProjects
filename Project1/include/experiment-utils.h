#ifndef __EXPERIMENT_UTILS_H__
#define __EXPERIMENT_UTILS_H__

#include <random>
#include <chrono>
#include <array>
#include <numeric>
#include <cstdint>
#include <iostream>
#include <x86intrin.h>
#include <sched.h>
#include <pthread.h>
#include <xmmintrin.h>
#include <pmmintrin.h>

using timespan = std::chrono::duration<double, std::nano>;

// EXPERIMENT DATA COLLECTION MACROS

#define TIME(...) { \
    std::cout << "Running: \e[32m" #__VA_ARGS__ "\e[m 10 times" << std::endl; \
    std::array<timespan, 10> timeDiffs; \
    std::array<uint64_t, 10> cyclesDiffs; \
    timespan minTime = timespan::max(); \
    timespan maxTime = timespan::zero(); \
    uint64_t minCycles = std::numeric_limits<uint64_t>::max(); \
    uint64_t maxCycles = 0; \
    for (int __i = 0; __i < 10; ++__i) \
    { \
        auto startTime = std::chrono::high_resolution_clock::now(); \
        auto startCycles = __rdtsc(); \
        __VA_ARGS__; \
        auto endCycles = __rdtsc(); \
        auto endTime = std::chrono::high_resolution_clock::now(); \
        timeDiffs[__i] = endTime - startTime; \
        cyclesDiffs[__i] = endCycles - startCycles; \
        \
        if (timeDiffs[__i] > maxTime) \
            maxTime = timeDiffs[__i]; \
        if (timeDiffs[__i] < minTime) \
            minTime = timeDiffs[__i]; \
        if (cyclesDiffs[__i] > maxCycles) \
            maxCycles = cyclesDiffs[__i]; \
        if (cyclesDiffs[__i] < minCycles) \
            minCycles = cyclesDiffs[__i]; \
    } \
    double avgTime = std::accumulate(timeDiffs.begin(), timeDiffs.end(), timespan(0)).count() / 10.0; \
    uint64_t avgCycles = std::accumulate(cyclesDiffs.begin(), cyclesDiffs.end(), 0) / 10; \
    std::cout << "Test complete. " \
                 "Average runtime: " << avgTime << " ns | " \
                 "Min runtime: " << minTime.count() << " ns | " \
                 "Max runtime: " << maxTime.count() << " ns | " \
                 "Average cycles: " << avgCycles << " | " \
                 "Min cycles: " << minCycles << " | " \
                 "Max cycles: " << maxCycles << std::endl; \
}

#define WARM3(ARR1, ARR2, ARR3, LENGTH) { \
    [[maybe_unused]] volatile float sink; \
    for (size_t i = 0; i < LENGTH; ++i) \
    { \
        sink = ARR1[i]; \
        sink = ARR2[i]; \
        sink = ARR3[i]; \
    } \
}

#define WARM2(ARR1, ARR2, LENGTH) { \
    [[maybe_unused]] volatile float sink; \
    for (size_t i = 0; i < LENGTH; ++i) \
    { \
        sink = ARR1[i]; \
        sink = ARR2[i]; \
    } \
}

// OTHER MACROS

// Random values generated in range [-LIMIT, LIMIT]
#define LIMIT 10000

// TESTS

template <typename T>
void streaming_fma(int scalar, T arr1[], T arr2[], T output[], size_t length, size_t stride = 1)
{
    for (size_t i = 0; i < length; i += stride)
        output[i] = scalar * arr1[i] + arr2[i];

    (void)output; // Prevent compiler from optimizing away `output`
}

template <typename T>
void reduction(T arr1[], T arr2[], size_t length)
{
    float output = 0;

    for (size_t i = 0; i < length; ++i)
        output += arr1[i] * arr2[i];

    (void)output; // Prevent compiler from optimizing away `output`
}

template <typename T>
void element_multiply(T arr1[], T arr2[], T output[], size_t length)
{
    for (size_t i = 0; i < length; ++i)
        output[i] = arr1[i] * arr2[i];

    (void)output; // Prevent compiler from optimizing away `output`
}

// HELPER FUNCTIONS

void performanceSetup()
{
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

    cpu_set_t cpuset;
    pthread_t thread = pthread_self();

    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);

    if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset))
        std::cerr << "Failed to pin to CPU core" << std::endl;
}

template <typename T, typename RNG>
void fillArrayRandomDecimal(T arr[], size_t length, RNG& generator)
{
    static std::uniform_real_distribution<T> dist(-LIMIT, LIMIT);
    for (size_t __i = 0; __i < length; ++__i)
        arr[__i] = dist(generator);
}

#endif
