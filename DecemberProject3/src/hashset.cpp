#include "hashset.h"
#include "MurmurHash3.h"
#include <cstdint>

void HashSet::insert(uint64_t key)
{
    size_t index = hash(key);
    for (uint64_t val : table[index])
    {
        if (val == key)
            return; // already exists, don't insert again
    }
    table[index].push_back(key);
}

void HashSet::remove(uint64_t key)
{
    size_t index = hash(key);
    table[index].remove(key);
}

bool HashSet::query(uint64_t key)
{
    size_t index = hash(key);
    for (uint64_t val : table[index])
    {
        if (val == key)
            return true;
    }
    return false;
}

size_t HashSet::hash(uint64_t key)
{
    __uint128_t tempHash;
    MurmurHash3_x64_128(&key, sizeof(uint64_t), SEED, &tempHash);
    return tempHash % size;
}
