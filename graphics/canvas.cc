#include "canvas.h"
namespace Graphics
{
    void Canvas::fill(u32 col)
    {
        for (u32 i = 0; i < width * height; i++)
        {
            pixels[i] = col;
        }
    }
    void Canvas::reset_belnded()
    {
        for (u32 i = 0; i < width * height; i++)
        {
            blended[i] = 0;
        }
    }
}
