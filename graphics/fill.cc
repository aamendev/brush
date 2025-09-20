#include "fill.h"
#include <queue>
#include <iostream>
namespace Graphics {

    Fill::Fill(i64 x, i64 y): mcolour(BLACK)
    {
        mx = x;
        my = y;
    }
    void Fill::draw(Canvas* c)
    {
        b8 cond = (c->wrap == WrapMode::Around
                        || (my < c->height && my > -1 && mx < c->width 
                            && mx > -1));
        if (!cond) return;
        u32 curr_col = c->pixels[Core::Utils::wrap_around(my, 0, c->height) * c->width 
            + Core::Utils::wrap_around(mx, 0, c->width)];
        if (curr_col == mcolour) return;
        b8* visited = new b8[c->width * c->height];
        for (u32 i = 0; i < c->width * c->height; i++)
        {
                visited[i] = false;
        }

        std::queue<std::pair<u32, u32>> curr_pixels;

        curr_pixels.push(
                {Core::Utils::wrap_around(mx, 0, c->width),
                Core::Utils::wrap_around(my, 0, c->height)});

        visited[curr_pixels.front().second * c->width + curr_pixels.front().first] 
            = true;
        while (!curr_pixels.empty())
        {
            auto [topx, topy] = curr_pixels.front();
            curr_pixels.pop();
            c->pixels[topy * c->width + topx] = mcolour;
            for (i64 j = -1; j < 2; j++)
            {
                for (i64 i = -1; i < 2; i++)
                {
                    cond = (c->wrap == WrapMode::Around
                        || ((topy + j) < c->height && (topy + j) > -1 && topx + i < c->width 
                            && topx + i > -1));
                    u32 idx = 
                        Core::Utils::wrap_around(topy + j, 0, c->height) * c->width 
                         + Core::Utils::wrap_around(topx + i, 0, c->width);

                    if (cond && !visited[idx] && c->pixels[idx] == curr_col)
                    {
                        visited[idx] = true;
                        curr_pixels.push({Core::Utils::wrap_around(topx + i, 0, c->width),
                                Core::Utils::wrap_around(topy + j, 0, c->height)});
                    }
                }
            }
        }
        delete[] visited;
    }
}
