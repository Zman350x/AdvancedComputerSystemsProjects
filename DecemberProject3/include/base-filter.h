#ifndef __BASE_FILTER_H__
#define __BASE_FILTER_H__

#include <cstdint>
#include <vector>

// Generated randomly, but consistent for all tests
#define SEED 0x5F368F9C

class BaseFilter
{
public:
    virtual void build(const std::vector<uint64_t>& keys) { }
    virtual void insert(uint64_t key) { }
    virtual void remove(uint64_t key) { }
    virtual bool query(uint64_t key) { return false; }
};

#endif
