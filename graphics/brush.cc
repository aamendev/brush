#include "brush.h"
#include "canvas.h"
#include "colours.h"
#include "utils.h"
#include <cmath>
#include <cstdlib>
namespace Graphics
{


    void Brush::draw(Canvas* c)
    {
        if (!mfilled)
        {
            std::cerr << "Brush not filled\n";
            return;
        }
        mfix_alpha ? alpha_fix_draw(c) : no_alpha_fix_draw(c);
    }
    void Brush::no_alpha_fix_draw(Canvas* c)
    {

        i64 new_x = mx - mr;
        i64 new_y = my - mr;

        i32 idx = 0;
        for (u32 j = 0; j < (2 * mr + 1); j++)
        {
            for (u32 i = 0; i < (2 * mr + 1); i++)
            {
                idx = Core::Utils::wrap_around(new_y + j, 0, c->height) 
                    * c->width +Core::Utils::wrap_around(new_x + i, 0, c->width);

                b8 cond = (c->wrap == WrapMode::Around
                        || (new_y + j < c->height && new_y + j > -1 && new_x + i < c->width 
                            && new_x + i > -1));
                if (cond)
                {
                    c->pixels[idx] = 
                    Utils::linear_interpolate(
                            c->pixels[idx],
                            mmask[j * (2 * mr + 1) + i]);
                }
            }
        }
    }
    void Brush::alpha_fix_draw(Canvas* c)
    {
        i64 new_x = mx - mr;
        i64 new_y = my - mr;

        for (u32 j = 0; j < (2 * mr + 1); j++)
        {
            for (u32 i = 0; i < (2 * mr + 1); i++)
            {
                int idx = Core::Utils::wrap_around(new_y + j, 0, c->height) 
                    * c->width +Core::Utils::wrap_around(new_x + i, 0, c->width);
                
                b8 cond = (c->wrap == WrapMode::Around
                        || (new_y + j < c->height && new_y + j > -1 && new_x + i < c->width 
                            && new_x + i > -1));
                if (!cond)
                {
                    continue;
                }
                if (c->blended[idx] == 0)
                {
                    c->blended[idx] = mmask[j * (2 * mr + 1) + i];
                    c->pixels[idx] = 
                    Utils::linear_interpolate(
                            c->pixels[idx],
                            mmask[j * (2 * mr + 1) + i]);
                    continue;
                }
                    u32 old_col;

                    u8 a = (f32)Utils::get_channel(c->blended[idx], ColourChannel::Alpha);
                    if (a == 0xff) continue;
                    u8 r = Utils::get_channel(c->blended[idx], ColourChannel::Red);
                    u8 g = Utils::get_channel(c->blended[idx], ColourChannel::Green);
                    u8 b = Utils::get_channel(c->blended[idx], ColourChannel::Blue);

                    Utils::set_channel(&old_col, 
                            Utils::get_original_component(Utils::get_channel(c->pixels[idx], ColourChannel::Red), r, a),
                            ColourChannel::Red);

                    Utils::set_channel(&old_col, 
                            Utils::get_original_component(Utils::get_channel(c->pixels[idx], ColourChannel::Green), g, a),
                            ColourChannel::Green);

                    Utils::set_channel(&old_col, 
                            Utils::get_original_component(Utils::get_channel(c->pixels[idx], ColourChannel::Blue), b, a),
                            ColourChannel::Blue);


                    Utils::set_channel(&old_col, 0xff, ColourChannel::Alpha);


                    c->pixels[idx] = 
                        Utils::linear_interpolate(
                                old_col,
                                c->blended[idx]);
            }
        }
    }

    SimpleBrush::SimpleBrush(i64 x, i64 y, u32 r): mcolour(BLACK)
    {
        mx = x;
        my = y;
        mr = r;
        mmask.resize( (2 * r + 1) * (2 * r + 1));
        mmask.assign(mmask.size(), 0);
        fill_mask();
        mfilled = true;
    }
    void SimpleBrush::fill_mask()
    {
        mfilled = true;
        int j = 0;
        int i = mr;

        i8 mul[2] = {-1, 1};
        while (i > -j)
        {
            for (int n = -i; n < 1; n++)
            {
                for (auto k : mul)
                {
                    for (auto m : mul)
                    {
                        mmask[(k * j + mr) * (2 * mr + 1) + m * n + mr] = mcolour; 
                        mmask[(k * n + mr) * (2 * mr + 1) + m * j + mr] = mcolour; 
                    }
                }
            }

            int i1 = i;
            int jj = j - 1;
            int i2 = i - 1;

            int v1 = i1 * i1 + jj * jj - mr * mr;
            int v2 = -(i2 * i2 + jj * jj - mr * mr);
            i = i1 * (v1 <= v2) + i2 * !(v1 <= v2);

            j--;
        }
    }


    void LinearBrush::fill_mask()
    {
        mfilled = true;
        int j = 0;
        int i = mr;

        u8 unpacked[4];
        Utils::unpack(unpacked, &mcolour);
        u32 new_col = mcolour;
        u8 origin_alpha = Utils::get_channel(mcolour, ColourChannel::Alpha);
        i8 mul[2] = {-1, 1};
        while (i > -j)
        {
            for (int n = -i; n < 1; n++)
            {
                Utils::set_channel(&new_col,
                        fmax(0, (1.0f - 
                                (f32)std::sqrt(j * j + n * n)/mr)) 
                        * origin_alpha,
                        ColourChannel::Alpha); 

                for (auto k : mul)
                {
                    for (auto m : mul)
                    {
                        mmask[(k * j + mr) * (2 * mr + 1) + m * n + mr] = new_col; 
                        mmask[(k * n + mr) * (2 * mr + 1) + m * j + mr] = new_col; 
                    }
                }
            }

            int i1 = i;
            int jj = j - 1;
            int i2 = i - 1;

            int v1 = i1 * i1 + jj * jj - mr * mr;
            int v2 = -(i2 * i2 + jj * jj - mr * mr);
            i = i1 * (v1 <= v2) + i2 * !(v1 <= v2);

            j--;
        }
    }
    void QuadraticBrush::fill_mask()
    {
        mfilled = true;
        int j = 0;
        int i = mr;

        f32 A = 1.0f/(mr * mr);
        f32 B = -2.0f/(mr);
        f32 d = 0.0f;

        u32 new_col = mcolour;
        u8 origin_alpha = Utils::get_channel(mcolour,ColourChannel::Alpha);
        while (i > -j)
        {
            for (int n = -i; n < i + 1; n++)
            {
                d = std::sqrt(j * j + n * n);

                Utils::set_channel(&new_col, 
                        fmax(0, A * d * d + B * d + 1.0f)
                        * origin_alpha
                        , ColourChannel::Alpha);

                mmask[(j + mr) * (2 * mr + 1) + n + mr] = new_col; 
                mmask[(mr - j) * (2 * mr + 1) + n + mr] = new_col; 
                mmask[(j + mr) * (2 * mr + 1) - n + mr] = new_col; 
                mmask[(mr - j) * (2 * mr + 1) - n + mr] = new_col; 

                mmask[(n + mr) * (2 * mr + 1) + j + mr] = new_col; 
                mmask[(mr - n) * (2 * mr + 1) + j + mr] = new_col; 
                mmask[(n + mr) * (2 * mr + 1) - j + mr] = new_col; 
                mmask[(mr - n) * (2 * mr + 1) - j + mr] = new_col; 
            }

            int i1 = i;
            int jj = j - 1;
            int i2 = i - 1;

            int v1 = i1 * i1 + jj * jj - mr * mr;
            int v2 = -(i2 * i2 + jj * jj - mr * mr);
            i = i1 * (v1 <= v2) + i2 * !(v1 <= v2);

            j--;
        }
    }
    void SprayBrush::fill_mask()
    {
        mfilled = true;
        int j = 0;
        int i = mr;
        int oldi = i;

        u8 unpacked[4];
        u32 new_col = 0;
        Utils::unpack(unpacked, &mcolour);
        u8 origin_alpha = unpacked[3];
        u8 curr_chance = 0;
        while (i > -j)
        {
            for (int n = -i; n < i + 1; n++)
            {
                new_col = 0;
                curr_chance = (rand() % 100) < mchances;
                unpacked[3] = origin_alpha * curr_chance;
                Utils::pack(unpacked, &new_col);

                mmask[(j + mr) * (2 * mr + 1) + n + mr] = new_col; 

                new_col = 0;
                curr_chance = (rand() % 100) < mchances;
                unpacked[3] = origin_alpha * curr_chance;
                Utils::pack(unpacked, &new_col);

                mmask[(mr -j + 1) * (2 * mr + 1) + n + mr] = new_col; 
            }
            for (int n = j; n < -(j - 1); n++)
            {
                if (oldi != i)
                {

                    new_col = 0;
                    curr_chance = (rand() % 100) < mchances;
                    unpacked[3] = origin_alpha * curr_chance;
                    Utils::pack(unpacked, &new_col);

                    mmask[(mr + i) * (2 * mr + 1) + n + mr] = new_col;

                    new_col = 0;
                    curr_chance = (rand() % 100) < mchances;
                    unpacked[3] = origin_alpha * curr_chance;
                    Utils::pack(unpacked, &new_col);

                    mmask[(mr -i + 1) * (2 * mr + 1) + n + mr] = new_col;
                }
            }


            oldi = i;
            int i1 = i;
            int jj = j - 1;
            int i2 = i - 1;

            int v1 = i1 * i1 + jj * jj - mr * mr;
            int v2 = -(i2 * i2 + jj * jj - mr * mr);
            i = i1 * (v1 <= v2) + i2 * !(v1 <= v2);

            j--;
        }
    }

    void SprayBrush::set_chances(u8 chances)
    {
        mchances = fmax(100, chances);
        if (mfilled) 
        {
            zero_mask();
            fill_mask();
        }
    }

    SmudgeBrush::SmudgeBrush(i64 x, i64 y, u32 r)
    {
        mx = x;
        my = y;
        mr = r;
        mmask.resize((2 * r + 1) * (2 * r + 1));
        mmask.assign(mmask.size(), 0);
        fill_mask();
        mfilled = true;
    }

    void SmudgeBrush::draw(Canvas* c)
    {
        if (!mfilled) 
        {
            std::cerr << "Mask is not filled\n";
            return;
        }
        i64 new_x = mx - mr;
        i64 new_y = my - mr;


        for (u32 j = 0; j < (2 * mr + 1); j++)
        {
            for (u32 i = 0; i < (2 * mr + 1); i++)
            {

                int idx = Core::Utils::wrap_around(new_y + j, 0, c->height) 
                    * c->width +Core::Utils::wrap_around(new_x + i, 0, c->width);
                b8 cond = (c->wrap == WrapMode::Around
                        || (new_y + j < c->height && new_y + j > -1 && new_x + i < c->width 
                            && new_x + i > -1));

                if (cond)
                {
                c->pixels[idx] = 
                    Utils::linear_interpolate(
                            c->pixels[idx],
                            mmask[j * (2 * mr + 1) + i]);
                }
            }
        }
        u32 col;
        u8 new_alpha;
        for (u32 j = 0; j < (2 * mr + 1); j++)
        {
            for (u32 i = 0; i < (2 * mr + 1); i++)
            {

                b8 cond = (c->wrap == WrapMode::Around
                        || (new_y + j < c->height && new_y + j > -1 && new_x + i < c->width 
                            && new_x + i > -1));

                col = c->pixels[Core::Utils::wrap_around(new_y + j, 0, c->height) 
                    * c->width + Core::Utils::wrap_around(new_x + i, 0, c->width)]
                    * cond;
                new_alpha = fmax(0, (1.0f - 
                            (f32)std::sqrt((j - mr) * (j - mr) + (i - mr) * (i-mr))/mr)) * 0xff * cond;

                Utils::set_channel(&col, new_alpha, ColourChannel::Alpha);
                mmask[j * (2 * mr + 1) + i] = col;
            }
        }
    }

    void SpeedBrush::draw(Canvas* c)
    {
        if (!mfilled)
        {
            std::cerr << "Mask Not Filled\n";
            return;
        }

        f32 dist = fmax(1.0f, ((mprev_x - mx) * (mprev_x - mx) + (mprev_y - my) * (mprev_y - my)));
        mprev_x = mx;
        mprev_y = my;
        set_r(mmin_r + 1.0f/(1.0f + std::log(1.0f + std::log(dist))) * mmax_r);

        mfix_alpha ? alpha_fix_draw(c) : no_alpha_fix_draw(c);
    }
}
