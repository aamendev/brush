#include "cursor.h"
namespace Core 
{
    Cursor::Cursor(i64 x, i64 y, u32 r): mx(x), my(y), mr(r), mcolour(BLACK)
    {
        curs_pixels = new u32[(2 * r + 1) * (2 * r + 1)];
        for (u32 i = 0; i < (2 * r + 1) * (2 * r + 1); i++)
        {
            curs_pixels[i] = mcolour;
        }
    }
}
