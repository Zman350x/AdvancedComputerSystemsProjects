#include "experiment-utils.h"
#include "blockedbloom.h"
#include "xorfilter.h"

int main()
{
    performanceSetup();

    XorFilter xorFilter;

    std::vector<uint64_t> keys;

    for (int i = 0; i < 10000; ++i)
        keys.emplace_back(i);

    xorFilter.build(keys);

    __NOP();

    return 0;
}
