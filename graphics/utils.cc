#include "utils.h"
namespace Graphics { namespace Utils {

    void unpack(u8* comp, u32* c)
    {
        comp[0] = *c&0XFF;
        comp[1] = (*c>>8) & 0XFF;
        comp[2] = (*c >> (8 * 2)) & 0xFF;
        comp[3] = (*c >> (8 * 3)) & 0xFF;
    }

    void pack(u8* comp, u32* c)
    {
        for (int i = 0; i < 4; i++)
        {
            *c |= comp[i]<<(8 * i);
        }
    }

    u32 linear_interpolate(u32 c1, u32 c2)
    {
        u8 comp1[4];
        unpack(comp1, &c1);
        u8 comp2[4];
        unpack(comp2, &c2);
        u32 new_col = 0;
        for (int i = 0; i < 3; i++)
        {
            comp1[i] = mix_component(comp1[i], comp2[i], comp2[3]);
            new_col |= comp1[i]<<(8 * i);
        }
        new_col |= 0xff<<(8 * 3);
        return new_col;
    }
}}
