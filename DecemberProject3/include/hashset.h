#ifndef __HASHSET_H__
#define __HASHSET_H__

#include <cstdint>
#include <list>
#include <vector>

class HashSet
{
public:
    HashSet(size_t size) : size(size), table(size) { }
    void insert(uint64_t key);
    void remove(uint64_t key);
    bool contains(uint64_t key);

private:
    const size_t size;
    std::vector<std::list<uint64_t>> table;
    size_t hash(uint64_t key);
};

#endif
