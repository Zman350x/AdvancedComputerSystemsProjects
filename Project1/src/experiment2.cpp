#include "experiment-utils.h"
#include <cstring>

// MACROS

// Test constants
#define ARR_LENGTH_MIN 5000
#define ARR_LENGTH_MAX 2000000
#define ARR_LENGTH_STEP 5000

// MAIN

int main(int argc, char* argv[])
{
    performanceSetup();

    // Setup use of random numbers
    std::random_device rd;
    std::mt19937 gen(rd());

    int scalar = std::uniform_int_distribution(-LIMIT, LIMIT)(gen);

    for (size_t i = ARR_LENGTH_MIN; i <= ARR_LENGTH_MAX; i += ARR_LENGTH_STEP)
    {
        // Generate input arrays

        std::cout << "Creating arrays of length " << i << std::endl;
        float f32_arr1[i];
        float f32_arr2[i];
        float f32_out[i];
        fillArrayRandomDecimal(f32_arr1, i, gen);
        fillArrayRandomDecimal(f32_arr2, i, gen);
        memset(f32_out, '\0', sizeof(float) * i);

        // KERNEL TESTS
        WARM3(f32_arr1, f32_arr2, f32_out, i);
        TIME(streaming_fma(scalar, f32_arr1, f32_arr2, f32_out, i));

        std::cout << std::endl;
    }

    return 0;
}
