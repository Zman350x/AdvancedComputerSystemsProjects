#include "experiment-utils.h"
#include <cstring>
#include <random>

// MACROS

// Test constants
#define ARR_LENGTH 500000

// TEMPLATES

template <typename T, typename RNG>
void streaming_fma_random(int scalar, T arr1[], T arr2[], T output[], size_t length, RNG& gen);

// MAIN

int main(int argc, char* argv[])
{
    performanceSetup();

    // Setup use of random numbers
    std::random_device rd;
    std::mt19937 gen(rd());

    int scalar = std::uniform_int_distribution(-LIMIT, LIMIT)(gen);

    // Generate input arrays

    std::cout << "Creating arrays" << std::endl;
    alignas(32) float f32_arr1[ARR_LENGTH];
    alignas(32) float f32_arr2[ARR_LENGTH];
    alignas(32) float f32_out[ARR_LENGTH];
    fillArrayRandomDecimal(f32_arr1, ARR_LENGTH, gen);
    fillArrayRandomDecimal(f32_arr2, ARR_LENGTH, gen);
    memset(f32_out, '\0', sizeof(float) * ARR_LENGTH);

    // KERNEL TESTS

    // Stride of 1
    WARM3(f32_arr1, f32_arr2, f32_out, i);
    TIME(streaming_fma(scalar, f32_arr1, f32_arr2, f32_out, ARR_LENGTH, 1));

    // Stride of 2
    WARM3(f32_arr1, f32_arr2, f32_out, i);
    TIME(streaming_fma(scalar, f32_arr1, f32_arr2, f32_out, ARR_LENGTH, 2));

    // Stride of 5
    WARM3(f32_arr1, f32_arr2, f32_out, i);
    TIME(streaming_fma(scalar, f32_arr1, f32_arr2, f32_out, ARR_LENGTH, 5));

    // Stride of 16
    WARM3(f32_arr1, f32_arr2, f32_out, i);
    TIME(streaming_fma(scalar, f32_arr1, f32_arr2, f32_out, ARR_LENGTH, 16));

    // Stride of 50
    WARM3(f32_arr1, f32_arr2, f32_out, i);
    TIME(streaming_fma(scalar, f32_arr1, f32_arr2, f32_out, ARR_LENGTH, 50));

    // Stride of random (between 1 and 5 every step, not the same throughout)
    WARM3(f32_arr1, f32_arr2, f32_out, i);
    TIME(streaming_fma_random(scalar, f32_arr1, f32_arr2, f32_out, ARR_LENGTH, gen));

    return 0;
}

// TESTS

template <typename T, typename RNG>
void streaming_fma_random(int scalar, T arr1[], T arr2[], T output[], size_t length, RNG& gen)
{
    static std::uniform_int_distribution dist(1, 5);

    for (size_t i = 0; i < length; i += dist(gen))
        output[i] = scalar * arr1[i] + arr2[i];

    (void)output; // Prevent compiler from optimizing away `output`
}

