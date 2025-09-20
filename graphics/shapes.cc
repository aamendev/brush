#include "shapes.h"

namespace Graphics
{
    void fill_circle(Canvas* canvas, int x, int y, int r, u32 colour)
    {
        int j = 0;
        int i = r;
        i8 mul[2] = {-1, 1};
        while (i > -j)
        {
            for (int n = -i; n < 1; n++)
            {
                for (auto k : mul)
                {
                    for (auto m : mul)
                    {
                        canvas->pixels[Core::Utils::wrap_around(k * j + y, 0, canvas->height) 
                            * canvas->width 
                            + Core::Utils::wrap_around(m * n + x, 0, canvas->width)] = colour;
                        canvas->pixels[Core::Utils::wrap_around(k * n + y, 0, canvas->height) 
                            * canvas->width 
                            + Core::Utils::wrap_around(m * j + x, 0, canvas->width)] = colour;
                    }
                }
            }
            
            int i1 = i;
            int jj = j - 1;
            int i2 = i - 1;

            int v1 = i1 * i1 + jj * jj - r * r;
            int v2 = -(i2 * i2 + jj * jj - r * r);
            i = i1 * (v1 <= v2) + i2 * !(v1 <= v2);

            j--;
        }

    }
    void outline_circle(Canvas *canvas, int x, int y, int r, u32 colour)
    {
        int j = 0;
        int i = r;
        i8 mul[2] = {-1, 1};

        while (i > -j)
        {
                for (auto k : mul)
                {
                    for (auto m : mul)
                    {
                        canvas->pixels[Core::Utils::wrap_around(k * j + y, 0, canvas->height) * canvas->width 
                            + Core::Utils::wrap_around(m * -i + x, 0, canvas->width)] = colour;
                        canvas->pixels[Core::Utils::wrap_around(k * -i + y, 0, canvas->height) * canvas->width 
                            + Core::Utils::wrap_around(m * j + x, 0, canvas->width)] = colour;
                    }
                }
            
            int i1 = i;
            int jj = j - 1;
            int i2 = i - 1;

            int v1 = i1 * i1 + jj * jj - r * r;
            int v2 = -(i2 * i2 + jj * jj - r * r);
            i = i1 * (v1 <= v2) + i2 * !(v1 <= v2);

            j--;
        }
    }
}
