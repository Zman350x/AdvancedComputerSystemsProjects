#include "blockedbloom.h"
#include "MurmurHash3.h"
#include <cmath>
#include <cstdint>

// Code based on: https://save-buffer.github.io/bloom_filter.html

uint8_t *BlockedBloom::getBlock(uint64_t key)
{
    int block_idx = hash(0, key) % numBlocks;
    return bitVector.data() + block_idx * CACHE_LINE_BITS / 8;
}

void BlockedBloom::insert(uint64_t key)
{
    uint8_t *block = getBlock(key);
    for(uint32_t i = 1; i < hashes; ++i)
    {
        uint64_t bit_pos = hash(i, key) % CACHE_LINE_BITS;
        uint64_t bit_idx = bit_pos % 8;
        uint64_t byte_idx = bit_pos / 8;
        block[byte_idx] |= (1 << bit_idx);
    }
}

bool BlockedBloom::query(uint64_t key)
{
    uint8_t *block = getBlock(key);
    for(uint32_t i = 1; i < hashes; ++i)
    {
        uint64_t bit_pos = hash(i, key) % CACHE_LINE_BITS;
        uint64_t bit_idx = bit_pos % 8;
        uint64_t byte_idx = bit_pos / 8;
        if (((block[byte_idx] >> bit_idx) & 1) == 0)
            return false;
    }
    return true;
}

uint32_t BlockedBloom::computeNumBits()
{
    return static_cast<uint32_t>(-1.44 * size * std::log2(epsilon) + 0.5);
}

uint32_t BlockedBloom::computeNumHashFns()
{
    return static_cast<uint32_t>(-std::log2(epsilon) + 0.5);
}

uint32_t BlockedBloom::hash(uint32_t id, uint64_t key)
{
    __uint128_t tempHash;
    MurmurHash3_x64_128(&key, sizeof(uint64_t), SEED + id, &tempHash);
    return (uint32_t) tempHash & 0xFFFFFFFF;
}
