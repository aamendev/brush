#ifndef SHAPES_H
#define SHAPES_H
#include "canvas.h"
#include "../types.h"
#include "../core/utils.h"
namespace Graphics
{
    void fill_circle(Canvas* canvas, int x, int y, int r, u32 colour);
    void outline_circle(Canvas* canvas, int x, int y, int r, u32 colour);
}
#endif
