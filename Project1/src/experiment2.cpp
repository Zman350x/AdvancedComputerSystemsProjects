#include "experiment-utils.h"

// MACROS

// Test constants
#define ARR_LENGTH_MIN 10000
#define ARR_LENGTH_MAX 5000000
#define ARR_LENGTH_STEP 10000

// PROTOTYPES

template <typename T>
void element_multiply(T arr1[], T arr2[], size_t length);

// MAIN

int main(int argc, char* argv[])
{
    performanceSetup();

    // Setup use of random numbers
    std::random_device rd;
    std::mt19937 gen(rd());


    for (size_t i = ARR_LENGTH_MIN; i <= ARR_LENGTH_MAX; i += ARR_LENGTH_STEP)
    {
        // Generate input arrays

        std::cout << "Creating arrays of length " << i << std::endl;
        float *f32_arr1 = new float[i];
        float *f32_arr2 = new float[i];
        fillArrayRandomDecimal(f32_arr1, i, gen);
        fillArrayRandomDecimal(f32_arr2, i, gen);

        // KERNEL TESTS
        TIME(element_multiply(f32_arr1, f32_arr2, i));

         std::cout << std::endl;

        delete [] f32_arr1;
        delete [] f32_arr2;
    }

    return 0;
}

// TESTS

template <typename T>
void element_multiply(T arr1[], T arr2[], size_t length)
{
    volatile float *output = new float[length];

    for (size_t i = 0; i < length; ++i)
        output[i] = arr1[i] * arr2[i];

    delete [] output;
}
