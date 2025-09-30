#include "experiment-utils.h"
#include <cstring>

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

    int scalar = std::uniform_int_distribution(-LIMIT, LIMIT)(gen);

    // Generate input arrays

    std::cout << "Creating arrays" << std::endl;
    float f32_arr1[ARR_LENGTH];
    float f32_arr2[ARR_LENGTH];
    float f32_out[ARR_LENGTH];
    fillArrayRandomDecimal(f32_arr1, ARR_LENGTH, gen);
    fillArrayRandomDecimal(f32_arr2, ARR_LENGTH, gen);
    memset(f32_out, '\0', sizeof(float) * ARR_LENGTH);

    // KERNEL TESTS
    WARM3(f32_arr1, f32_arr2, f32_out, i);
    TIME(streaming_fma(scalar, f32_arr1, f32_arr2, f32_out, ARR_LENGTH));

    return 0;
}
