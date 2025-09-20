#ifndef UTILS_H
#define UTILS_H
#include "../types.h"
#include "colours.h"
#include <iostream>
#include <math.h>

namespace Graphics { namespace Utils { 
    void unpack(u8* comp, u32* c);
    void pack(u8* comp, u32* c);

    inline u8 mix_component(u8 c1, u8 c2, u8 a)
    {
        return (1.0f - (f32)a/0xff) * c1 + (f32)a/0xff * c2 + 0.5f;
    }
    // nc = (1-a) c1 + a c2
    // nc = c1 - ac1 + ac2
    // nc = c1 + (c2 - c1) (a/0xff)
    inline u8 get_original_component(u8 mixed, u8 c2, u8 a)
    {
        if (a == 0xff) return c2;
        auto ret = fmin(0xff, fmax(0, (f32)((f32)mixed - (f32)a/0xff * (f32)c2) / (1.0f - (f32)a/0xff) + 0.5f));
        return ret;
    }
    u32 linear_interpolate(u32 c1, u32 c2);
    inline u8 get_channel(u32 col, ColourChannel channel)
    {
        return (col>>(8 * (u8)channel))&0xff;
    }
    inline void set_channel(u32* col, u8 val, ColourChannel channel)
    {
        (*col)&= (~((0xff)<<(8 * (u8)channel)));
        (*col)|= (val<<(8 * (u8)channel));
    }
}}
#endif
