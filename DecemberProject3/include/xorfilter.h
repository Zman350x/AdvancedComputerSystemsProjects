#ifndef __XORFILTER_H__
#define __XORFILTER_H__

#include <cstdint>
#include "base-filter.h"

class XorFilter : BaseFilter
{
public:
    XorFilter(uint32_t size) : size(size) { }
    void build(const std::vector<uint64_t>& keys) override;
    bool query(uint64_t key) override;

private:
    uint32_t hash(uint64_t key);
    const uint32_t size;

    std::vector<uint8_t> fingerprints;
};

#endif
