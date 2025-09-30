#include "experiment-utils.h"

// MACROS

#define MISALIGN(...) reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(__VA_ARGS__) + 1)

// Test constants
// NOTAIL is a multiple of 256, whereas TAIL is not
#define NOTAIL_LENGTH 524288
#define TAIL_LENGTH 524475

// MAIN

int main(int argc, char* argv[])
{
    performanceSetup();

    // Setup use of random numbers
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate input arrays

    std::cout << "Creating aligned, no-tail arrays" << std::endl;
    alignas(32) float f32_aligned_notail_arr1[NOTAIL_LENGTH];
    alignas(32) float f32_aligned_notail_arr2[NOTAIL_LENGTH];
    alignas(32) float f32_aligned_notail_out[NOTAIL_LENGTH] = { 0 };
    fillArrayRandomDecimal(f32_aligned_notail_arr1, NOTAIL_LENGTH, gen);
    fillArrayRandomDecimal(f32_aligned_notail_arr2, NOTAIL_LENGTH, gen);

    std::cout << "Creating aligned, tail arrays" << std::endl;
    alignas(32) float f32_aligned_tail_arr1[TAIL_LENGTH];
    alignas(32) float f32_aligned_tail_arr2[TAIL_LENGTH];
    alignas(32) float f32_aligned_tail_out[TAIL_LENGTH] = { 0 };
    fillArrayRandomDecimal(f32_aligned_tail_arr1, TAIL_LENGTH, gen);
    fillArrayRandomDecimal(f32_aligned_tail_arr2, TAIL_LENGTH, gen);

    std::cout << "Creating misaligned, no-tail arrays" << std::endl;
    alignas(32) float _f32_misaligned_notail_arr1_src[NOTAIL_LENGTH + 1];
    float *f32_misaligned_notail_arr1 = MISALIGN(_f32_misaligned_notail_arr1_src);
    alignas(32) float _f32_misaligned_notail_arr2_src[NOTAIL_LENGTH + 1];
    float *f32_misaligned_notail_arr2 = MISALIGN(_f32_misaligned_notail_arr2_src);
    alignas(32) float _f32_misaligned_notail_out_src[NOTAIL_LENGTH + 1] = { 0 };
    float *f32_misaligned_notail_out = MISALIGN(_f32_misaligned_notail_out_src);
    fillArrayRandomDecimal(f32_misaligned_notail_arr1, NOTAIL_LENGTH, gen);
    fillArrayRandomDecimal(f32_misaligned_notail_arr2, NOTAIL_LENGTH, gen);

    std::cout << "Creating misaligned, tail arrays" << std::endl;
    alignas(32) float _f32_misaligned_tail_arr1_src[TAIL_LENGTH + 1];
    float *f32_misaligned_tail_arr1 = MISALIGN(_f32_misaligned_tail_arr1_src);
    alignas(32) float _f32_misaligned_tail_arr2_src[TAIL_LENGTH + 1];
    float *f32_misaligned_tail_arr2 = MISALIGN(_f32_misaligned_tail_arr2_src);
    alignas(32) float _f32_misaligned_tail_out_src[TAIL_LENGTH + 1] = { 0 };
    float *f32_misaligned_tail_out = MISALIGN(_f32_misaligned_tail_out_src);
    fillArrayRandomDecimal(f32_misaligned_tail_arr1, TAIL_LENGTH, gen);
    fillArrayRandomDecimal(f32_misaligned_tail_arr2, TAIL_LENGTH, gen);

    std::cout << "All arrays created\n" << std::endl;

    // KERNEL TESTS

    int scalar = std::uniform_int_distribution(-LIMIT, LIMIT)(gen);

    // TIMING

    WARM3(f32_aligned_notail_arr1, f32_aligned_notail_arr2, f32_aligned_notail_out, NOTAIL_LENGTH);
    TIME(streaming_fma(scalar, f32_aligned_notail_arr1, f32_aligned_notail_arr2, f32_aligned_notail_out, NOTAIL_LENGTH));
    WARM3(f32_aligned_tail_arr1, f32_aligned_tail_arr2, f32_aligned_tail_out, TAIL_LENGTH);
    TIME(streaming_fma(scalar, f32_aligned_tail_arr1, f32_aligned_tail_arr2, f32_aligned_tail_out, TAIL_LENGTH));
    WARM3(f32_misaligned_notail_arr1, f32_misaligned_notail_arr2, f32_misaligned_notail_out, NOTAIL_LENGTH);
    TIME(streaming_fma(scalar, f32_misaligned_notail_arr1, f32_misaligned_notail_arr2, f32_misaligned_notail_out, NOTAIL_LENGTH));
    WARM3(f32_misaligned_tail_arr1, f32_misaligned_tail_arr2, f32_misaligned_tail_out, TAIL_LENGTH);
    TIME(streaming_fma(scalar, f32_misaligned_tail_arr1, f32_misaligned_tail_arr2, f32_misaligned_tail_out, TAIL_LENGTH));

    return 0;
}
