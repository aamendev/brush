#ifndef CORE_UTIL_H
#define CORE_UTIL_H
#include "../types.h"
#include <iostream>
namespace Core { namespace Utils {
    inline u32 wrap_around(i64 val, u32 min, u32 max)
    {
        if (val == 0) return val;
        return (max * (-(val/max) + 1) + val) * (val < min) + 
            ((val % max) + min) * (val >= max)
            + val * (val > min && val < (int)max);
    }
}}
#endif
