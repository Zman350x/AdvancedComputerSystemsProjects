#include "xorfilter.h"
#include "MurmurHash3.h"
#include <cstdint>

// Based in part on https://github.com/FastFilter/xor_singleheader/blob/master/include/xorfilter.h

bool XorFilter::build(const std::vector<uint64_t>& keys)
{
    size = keys.size();
    if (size == 0)
        return false;

    capacity = 32 + 1.23 * size;
    capacity = capacity / 3 * 3;
    blockLength = capacity / 3;

    fingerprints.clear();
    fingerprints.resize(capacity);

    std::vector<KeyIndex> stack(size);
    std::vector<KeyIndex> q0(blockLength);
    std::vector<KeyIndex> q1(blockLength);
    std::vector<KeyIndex> q2(blockLength);
    std::vector<XorSet> sets0(blockLength);
    std::vector<XorSet> sets1(blockLength);
    std::vector<XorSet> sets2(blockLength);

    uint32_t i;
    for (i = 0; i < 1024; ++i) // 1024 is a timout
    {
        for (size_t j = 0; j < size; ++j)
        {
            XorHashes hashes = getHashes(keys[j]);
            sets0[hashes.h0].xorMask ^= hashes.h;
            ++sets0[hashes.h0].count;
            sets1[hashes.h1].xorMask ^= hashes.h;
            ++sets1[hashes.h1].count;
            sets2[hashes.h2].xorMask ^= hashes.h;
            ++sets2[hashes.h2].count;
        }

        size_t q0Size = 0, q1Size = 0, q2Size = 0;

        for (size_t j = 0; j < blockLength; ++j)
        {
            if (sets0[i].count == 1)
            {
                q0[q0Size].index = (uint32_t)i;
                q0[q0Size].hash = sets0[i].xorMask;
                ++q0Size;
            }

            if (sets1[i].count == 1)
            {
                q1[q1Size].index = (uint32_t)i;
                q1[q1Size].hash = sets1[i].xorMask;
                ++q1Size;
            }

            if (sets2[i].count == 1)
            {
                q2[q2Size].index = (uint32_t)i;
                q2[q2Size].hash = sets2[i].xorMask;
                ++q2Size;
            }
        }

        size_t stackSize = 0;
        while (q0Size + q1Size + q2Size > 0)
        {
            while (q0Size > 0)
            {
                KeyIndex keyIndex = q0[--q0Size];
                if (sets0[keyIndex.index].count == 0)
                    continue;

                uint32_t h1 = getH1(keyIndex.hash);
                uint32_t h2 = getH2(keyIndex.hash);


            }
        }
    }

    if (i == 1024) // if timed out
        return false;

    return true;
}

bool XorFilter::query(uint64_t key)
{
    return false;
}

static inline uint64_t rotl64(uint64_t n, unsigned int c)
{
    return (n << (c & 63U)) | (n >> ((-c) & 63U));
}

static inline uint32_t reduce(uint32_t hash, uint32_t n) {
    // http://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
    return (uint32_t)(((uint64_t)hash * n) >> 32U);
}

uint64_t XorFilter::hash(uint32_t id, uint64_t key)
{
    __uint128_t tempHash;
    MurmurHash3_x64_128(&key, sizeof(uint64_t), SEED + id, &tempHash);
    return (uint64_t) tempHash;
}

XorHashes XorFilter::getHashes(uint64_t key)
{
    XorHashes hashes;
    hashes.h = hash(0, key);
    hashes.h0 = reduce((uint32_t) hashes.h, blockLength);
    hashes.h1 = reduce((uint32_t) rotl64(hashes.h, 21), blockLength);
    hashes.h2 = reduce((uint32_t) rotl64(hashes.h, 42), blockLength);
    return hashes;
}

uint32_t XorFilter::getH0(uint64_t hash)
{
    return reduce((uint32_t) hash, blockLength);
}

uint32_t XorFilter::getH1(uint64_t hash)
{
    return reduce((uint32_t) rotl64(hash, 21), blockLength);
}

uint32_t XorFilter::getH2(uint64_t hash)
{
    return reduce((uint32_t) rotl64(hash, 42), blockLength);
}
