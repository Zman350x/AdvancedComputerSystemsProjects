#include "experiment-utils.h"

// MACROS

// Test constants
#define ARR_LENGTH 500000

// MAIN

int main(int argc, char* argv[])
{
    performanceSetup();

    // Setup use of random numbers
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate input arrays

    std::cout << "Creating float32 arrays" << std::endl;
    alignas(32) float f32_arr1[ARR_LENGTH];
    alignas(32) float f32_arr2[ARR_LENGTH];
    alignas(32) float f32_out[ARR_LENGTH] = { 0 };
    fillArrayRandomDecimal(f32_arr1, ARR_LENGTH, gen);
    fillArrayRandomDecimal(f32_arr2, ARR_LENGTH, gen);

    std::cout << "Creating float64 arrays" << std::endl;
    alignas(32) double f64_arr1[ARR_LENGTH];
    alignas(32) double f64_arr2[ARR_LENGTH];
    alignas(32) double f64_out[ARR_LENGTH] = { 0 };
    fillArrayRandomDecimal(f64_arr1, ARR_LENGTH, gen);
    fillArrayRandomDecimal(f64_arr2, ARR_LENGTH, gen);

    std::cout << "All arrays created\n" << std::endl;

    // KERNEL TESTS

    int scalar = std::uniform_int_distribution(-LIMIT, LIMIT)(gen);

    // TIMING

    WARM3(f32_arr1, f32_arr2, f32_out, ARR_LENGTH);
    TIME(streaming_fma(scalar, f32_arr1, f32_arr2, f32_out, ARR_LENGTH));

    WARM3(f64_arr1, f64_arr2, f64_out, ARR_LENGTH);
    TIME(streaming_fma(scalar, f64_arr1, f64_arr2, f64_out, ARR_LENGTH));

    return 0;
}
