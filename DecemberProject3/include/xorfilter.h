#ifndef __XORFILTER_H__
#define __XORFILTER_H__

#include <cstdint>
#include "base-filter.h"

struct KeyIndex
{
    uint64_t hash;
    size_t index;
};

struct XorSet
{
    uint64_t xorMask;
    uint32_t count;
};

struct XorHashes
{
    uint64_t h;
    uint32_t h0;
    uint32_t h1;
    uint32_t h2;
};

class XorFilter : BaseFilter
{
public:
    bool build(std::vector<uint64_t> keys) override;
    bool query(uint64_t key) override;

private:
    uint64_t hash(uint32_t id, uint64_t key);
    XorHashes getHashes(uint64_t key);
    uint32_t getH0(uint64_t hash);
    uint32_t getH1(uint64_t hash);
    uint32_t getH2(uint64_t hash);
    size_t size, capacity;
    uint32_t blockLength, id;

    std::vector<uint8_t> fingerprints;
};

#endif
