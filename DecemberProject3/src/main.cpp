#include "experiment-utils.h"
#include "blockedbloom.h"

int main()
{
    performanceSetup();

    BlockedBloom bloom(10000, 0.05);

    for (int i = 0; i < 10000; ++i)
        bloom.insert(i);

    for (int i = 0; i < 10000; ++i)
        bloom.query(i);

    __NOP();

    return 0;
}
