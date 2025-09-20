#ifndef CANVAS_H
#define CANVAS_H
#include "../types.h"
namespace Graphics
{
    enum class WrapMode
    {
        Clamp,
        Around,
        None,
    };
    struct Canvas
    {
        u32 width;
        u32 height;
        u32* pixels;
        u32* blended;
        WrapMode wrap;
        void fill(u32 col);
        void reset_belnded();
    };
}
#endif
