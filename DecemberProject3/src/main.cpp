#include "experiment-utils.h"
#include "blockedbloom.h"
#include "xorfilter.h"

int main()
{
    performanceSetup();

    // BLOOM

    BlockedBloom bloom(4, 0.01);

    bloom.insert(2);
    bloom.insert(8);
    bloom.insert(40345);
    bloom.insert(9493);

    std::cout << "Bloom:\n" <<
                 bloom.query(2) << '\n' <<
                 bloom.query(8) << '\n' <<
                 bloom.query(17) << '\n' <<
                 bloom.query(40344) << '\n' <<
                 bloom.query(40345) << '\n' <<
                 bloom.query(94) << '\n' <<
                 bloom.query(9400) << '\n' <<
                 bloom.query(9493) << '\n' <<
                 bloom.query(9496) << std::endl;

    // XOR

    XorFilter xorFilter;

    std::vector<uint64_t> keys(4);

    keys[0] = 2;
    keys[1] = 8;
    keys[2] = 40345;
    keys[3] = 9493;

    xorFilter.build(keys);

    std::cout << "XOR:\n" <<
                 xorFilter.query(2) << '\n' <<
                 xorFilter.query(8) << '\n' <<
                 xorFilter.query(17) << '\n' <<
                 xorFilter.query(40344) << '\n' <<
                 xorFilter.query(40345) << '\n' <<
                 xorFilter.query(94) << '\n' <<
                 xorFilter.query(9400) << '\n' <<
                 xorFilter.query(9493) << '\n' <<
                 xorFilter.query(9496) << std::endl;

    return 0;
}
