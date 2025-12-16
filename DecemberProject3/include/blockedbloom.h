#ifndef __BLOCKEDBLOOM_H__
#define __BLOCKEDBLOOM_H__

#include <cstdint>
#include <vector>
#include "base-filter.h"
#include "experiment-utils.h"

// Code based on: https://save-buffer.github.io/bloom_filter.html

class BlockedBloom : BaseFilter
{
public:
    BlockedBloom(uint32_t size, float eps) : epsilon(eps), size(size)
    {
        bits = computeNumBits();
        hashes = computeNumHashFns();
        numBlocks = (bits + CACHE_LINE_BITS - 1) / CACHE_LINE_BITS;

        bitVector.resize(numBlocks * CACHE_LINE_BITS / 8);
    }
    void insert(uint64_t key) override;
    bool query(uint64_t key) override;

private:
    uint32_t computeNumHashFns();
    uint32_t computeNumBits();
    uint8_t *getBlock(uint64_t key);
    uint32_t hash(uint32_t id, uint64_t key);

    uint32_t bits, hashes, numBlocks;
    const float epsilon;
    const uint32_t size;
    std::vector<uint8_t> bitVector;
};

#endif
