#include "experiment-utils.h"

// MACROS

// Test constants
// Set to a little under 1/3 the size of the cache, so both arr1, arr2, and the
// output array can fit within the cache (+ a little overhead for other data)
// Technically, a single array of size L3_ARR_LENGTH could fit in L2, but all
// tests in this file require at least 2 arrays, so it should always have to use
// the L3 cache
#define L1_ARR_LENGTH 20000
#define L2_ARR_LENGTH 500000
#define L3_ARR_LENGTH 1000000
#define DRAM_ARR_LENGTH 5000000

// MAIN

int main(int argc, char* argv[])
{
    performanceSetup();

    // Setup use of random numbers
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate input arrays

    std::cout << "Creating L1 arrays" << std::endl;
    float f32_l1_arr1[L1_ARR_LENGTH];
    float f32_l1_arr2[L1_ARR_LENGTH];
    float f32_l1_out[L1_ARR_LENGTH] = { 0 };
    fillArrayRandomDecimal(f32_l1_arr1, L1_ARR_LENGTH, gen);
    fillArrayRandomDecimal(f32_l1_arr2, L1_ARR_LENGTH, gen);

    std::cout << "Creating L2 arrays" << std::endl;
    float f32_l2_arr1[L2_ARR_LENGTH];
    float f32_l2_arr2[L2_ARR_LENGTH];
    float f32_l2_out[L2_ARR_LENGTH] = { 0 };
    fillArrayRandomDecimal(f32_l2_arr1, L2_ARR_LENGTH, gen);
    fillArrayRandomDecimal(f32_l2_arr2, L2_ARR_LENGTH, gen);

    std::cout << "Creating L3 arrays" << std::endl;
    float f32_l3_arr1[L3_ARR_LENGTH];
    float f32_l3_arr2[L3_ARR_LENGTH];
    float f32_l3_out[L3_ARR_LENGTH] = { 0 };
    fillArrayRandomDecimal(f32_l3_arr1, L3_ARR_LENGTH, gen);
    fillArrayRandomDecimal(f32_l3_arr2, L3_ARR_LENGTH, gen);

    std::cout << "Creating DRAM arrays" << std::endl;
    float f32_dram_arr1[DRAM_ARR_LENGTH];
    float f32_dram_arr2[DRAM_ARR_LENGTH];
    float f32_dram_out[DRAM_ARR_LENGTH] = { 0 };
    fillArrayRandomDecimal(f32_dram_arr1, DRAM_ARR_LENGTH, gen);
    fillArrayRandomDecimal(f32_dram_arr2, DRAM_ARR_LENGTH, gen);

    std::cout << "All arrays created\n" << std::endl;

    // KERNEL TESTS

    int scalar = std::uniform_int_distribution(-LIMIT, LIMIT)(gen);

    // TIMING

    WARM3(f32_l1_arr1, f32_l1_arr2, f32_l1_out, L1_ARR_LENGTH);
    TIME(streaming_fma(scalar, f32_l1_arr1, f32_l1_arr2, f32_l1_out, L1_ARR_LENGTH));
    WARM3(f32_l2_arr1, f32_l2_arr2, f32_l2_out, L2_ARR_LENGTH);
    TIME(streaming_fma(scalar, f32_l2_arr1, f32_l2_arr2, f32_l2_out, L2_ARR_LENGTH));
    WARM3(f32_l3_arr1, f32_l3_arr2, f32_l3_out, L3_ARR_LENGTH);
    TIME(streaming_fma(scalar, f32_l3_arr1, f32_l3_arr2, f32_l3_out, L3_ARR_LENGTH));
    WARM3(f32_dram_arr1, f32_dram_arr2, f32_dram_out, DRAM_ARR_LENGTH);
    TIME(streaming_fma(scalar, f32_dram_arr1, f32_dram_arr2, f32_dram_out, DRAM_ARR_LENGTH));

    WARM3(f32_l1_arr1, f32_l1_arr2, f32_l1_out, L1_ARR_LENGTH);
    TIME(reduction(f32_l1_arr1, f32_l1_arr2, L1_ARR_LENGTH));
    WARM3(f32_l2_arr1, f32_l2_arr2, f32_l2_out, L2_ARR_LENGTH);
    TIME(reduction(f32_l2_arr1, f32_l2_arr2, L2_ARR_LENGTH));
    WARM3(f32_l3_arr1, f32_l3_arr2, f32_l3_out, L3_ARR_LENGTH);
    TIME(reduction(f32_l3_arr1, f32_l2_arr2, L3_ARR_LENGTH));
    WARM3(f32_dram_arr1, f32_dram_arr2, f32_dram_out, DRAM_ARR_LENGTH);
    TIME(reduction(f32_dram_arr1, f32_dram_arr2, DRAM_ARR_LENGTH));

    WARM3(f32_l1_arr1, f32_l1_arr2, f32_l1_out, L1_ARR_LENGTH);
    TIME(element_multiply(f32_l1_arr1, f32_l1_arr2, f32_l1_out, L1_ARR_LENGTH));
    WARM3(f32_l2_arr1, f32_l2_arr2, f32_l2_out, L2_ARR_LENGTH);
    TIME(element_multiply(f32_l2_arr1, f32_l2_arr2, f32_l2_out, L2_ARR_LENGTH));
    WARM3(f32_l3_arr1, f32_l3_arr2, f32_l3_out, L3_ARR_LENGTH);
    TIME(element_multiply(f32_l3_arr1, f32_l2_arr2, f32_l2_out, L3_ARR_LENGTH));
    WARM3(f32_dram_arr1, f32_dram_arr2, f32_dram_out, DRAM_ARR_LENGTH);
    TIME(element_multiply(f32_dram_arr1, f32_dram_arr2, f32_dram_out, DRAM_ARR_LENGTH));

    return 0;
}
