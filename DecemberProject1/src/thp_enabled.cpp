#include "experiment-utils.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
#include <chrono>

// 2 MiB alignment, the size of a hugepage
#define MEM_ALIGNMENT 2097152

// 2 GiB memory allocated
#define MEM_SIZE 2147483648

int main()
{
    // Start timing
    auto startTime = std::chrono::high_resolution_clock::now();
    auto startCycles = __rdtsc();

    uint8_t *mem;
    posix_memalign((void**) &mem, MEM_ALIGNMENT, MEM_SIZE);
    madvise(mem, MEM_SIZE, MADV_HUGEPAGE); // THIS LINE ENABLES THP
    memset(mem, 1, MEM_SIZE);

    // random-like small steps
    for (size_t i = 0; i < MEM_SIZE; i += 64)
        mem[i] += 1;

    // Stop timing
    auto endCycles = __rdtsc();
    auto endTime = std::chrono::high_resolution_clock::now();
    timespanSec timeDiff = endTime - startTime;
    uint64_t cycles = endCycles - startCycles;

    std::cout << "thp_enabled" << "," << timeDiff.count() << "," << cycles << std::endl;

    free(mem);
    return 0;
}
