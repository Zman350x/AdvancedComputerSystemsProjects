#include "hashset.h"
#include <cstdint>
#include <functional>

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

bool HashSet::contains(uint64_t key)
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
    return std::hash<uint64_t>{}(key) % size;
}
