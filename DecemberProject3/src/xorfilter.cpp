#include "xorfilter.h"
#include "MurmurHash3.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>

// Based in part on https://github.com/FastFilter/xor_singleheader/blob/master/include/xorfilter.h

static inline uint64_t rotl64(uint64_t n, unsigned int c);
static inline uint32_t reduce(uint32_t hash, uint32_t n);
static inline uint64_t xorFingerprint(uint64_t hash);
static size_t xorSortAndRemoveDup(std::vector<uint64_t>& keys);

bool XorFilter::build(std::vector<uint64_t> keys)
{
    size = keys.size();
    if (size == 0)
        return false;

    capacity = 32 + 1.23 * size;
    capacity = capacity / 3 * 3;
    blockLength = capacity / 3;

    id = 0;

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
        if (i == 10)
            size = xorSortAndRemoveDup(keys);

        sets0.clear();
        sets0.resize(blockLength);
        sets1.clear();
        sets1.resize(blockLength);
        sets2.clear();
        sets2.resize(blockLength);

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
            if (sets0[j].count == 1)
            {
                q0[q0Size].index = (uint32_t)j;
                q0[q0Size].hash = sets0[j].xorMask;
                ++q0Size;
            }

            if (sets1[j].count == 1)
            {
                q1[q1Size].index = (uint32_t)j;
                q1[q1Size].hash = sets1[j].xorMask;
                ++q1Size;
            }

            if (sets2[j].count == 1)
            {
                q2[q2Size].index = (uint32_t)j;
                q2[q2Size].hash = sets2[j].xorMask;
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

                stack[stackSize++] = keyIndex;

                sets1[h1].xorMask ^= keyIndex.hash;
                --sets1[h1].count;
                if (sets1[h1].count == 1)
                {
                    q1[q1Size].index = h1;
                    q1[q1Size].hash = sets1[h1].xorMask;
                    ++q1Size;
                }

                sets2[h2].xorMask ^= keyIndex.hash;
                --sets2[h2].count;
                if (sets2[h2].count == 1)
                {
                    q2[q2Size].index = h2;
                    q2[q2Size].hash = sets2[h2].xorMask;
                    ++q2Size;
                }
            }
            while (q1Size > 0)
            {
                KeyIndex keyIndex = q1[--q1Size];
                if (sets1[keyIndex.index].count == 0)
                    continue;

                uint32_t h0 = getH0(keyIndex.hash);
                uint32_t h2 = getH2(keyIndex.hash);
                keyIndex.index += blockLength;

                stack[stackSize++] = keyIndex;

                sets0[h0].xorMask ^= keyIndex.hash;
                --sets0[h0].count;
                if (sets0[h0].count == 1)
                {
                    q0[q0Size].index = h0;
                    q0[q0Size].hash = sets0[h0].xorMask;
                    ++q0Size;
                }

                sets2[h2].xorMask ^= keyIndex.hash;
                --sets2[h2].count;
                if (sets2[h2].count == 1)
                {
                    q2[q2Size].index = h2;
                    q2[q2Size].hash = sets2[h2].xorMask;
                    ++q2Size;
                }
            }
            while (q2Size > 0)
            {
                KeyIndex keyIndex = q2[--q2Size];
                if (sets2[keyIndex.index].count == 0)
                    continue;

                uint32_t h0 = getH0(keyIndex.hash);
                uint32_t h1 = getH1(keyIndex.hash);
                keyIndex.index += 2 * blockLength;

                stack[stackSize++] = keyIndex;

                sets0[h0].xorMask ^= keyIndex.hash;
                --sets0[h0].count;
                if (sets0[h0].count == 1)
                {
                    q0[q0Size].index = h0;
                    q0[q0Size].hash = sets0[h0].xorMask;
                    ++q0Size;
                }

                sets1[h1].xorMask ^= keyIndex.hash;
                --sets1[h1].count;
                if (sets1[h1].count == 1)
                {
                    q1[q1Size].index = h1;
                    q1[q1Size].hash = sets1[h1].xorMask;
                    ++q1Size;
                }
            }
        }

        if (stackSize == size)
            break; // Success

        ++id;
    }

    if (i == 1024) // if timed out
        return false;

    uint8_t *fingerprints0 = fingerprints.data();
    uint8_t *fingerprints1 = fingerprints.data() + blockLength;
    uint8_t *fingerprints2 = fingerprints.data() + 2 * blockLength;

    size_t stackSize = size;
    while (stackSize > 0)
    {
        KeyIndex keyIndex = stack[--stackSize];
        uint64_t f = xorFingerprint(keyIndex.hash);
        if (keyIndex.index < blockLength)
            f ^= (uint32_t)fingerprints1[getH1(keyIndex.hash)] ^ fingerprints2[getH2(keyIndex.hash)];
        else if (keyIndex.index < 2 * blockLength)
            f ^= (uint32_t)fingerprints0[getH0(keyIndex.hash)] ^ fingerprints2[getH2(keyIndex.hash)];
        else
            f ^= (uint32_t)fingerprints0[getH0(keyIndex.hash)] ^ fingerprints1[getH1(keyIndex.hash)];

        fingerprints[keyIndex.index] = (uint8_t) f;
    }

    return true;
}

bool XorFilter::query(uint64_t key)
{
    XorHashes hashes = getHashes(key);
    hashes.h1 += blockLength;
    hashes.h2 += 2 * blockLength;
    uint8_t f = (uint8_t) xorFingerprint(hashes.h);
    return f == ((uint32_t) fingerprints[hashes.h0] ^ fingerprints[hashes.h1] ^ fingerprints[hashes.h2]);
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
    hashes.h = hash(id, key);
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

static inline uint64_t rotl64(uint64_t n, unsigned int c)
{
    return (n << (c & 63U)) | (n >> ((-c) & 63U));
}

static inline uint32_t reduce(uint32_t hash, uint32_t n)
{
    // http://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
    return (uint32_t)(((uint64_t)hash * n) >> 32U);
}

static inline uint64_t xorFingerprint(uint64_t hash)
{
    return (hash ^ (hash >> 32U));
}

static size_t xorSortAndRemoveDup(std::vector<uint64_t>& keys)
{
    std::sort(keys.begin(), keys.end());
    size_t j = 1;
    for(size_t i = 1; i < keys.size(); ++i) {
        if(keys[i] != keys[i-1])
        {
            keys[j] = keys[i];
            ++j;
        }
    }
    return j;
}
