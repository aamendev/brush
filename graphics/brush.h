#ifndef BRUSH_H
#define BRUSH_H
#include "../types.h"
#include "canvas.h"
#include <math.h>
#include "colours.h"
#include "tool.h"
#include "../core/utils.h"
#include <vector>
namespace Graphics{
    class Brush : public Tool
    {
        public:
            virtual void fill_mask() = 0;
            virtual void zero_mask() = 0;

            inline void set_r(u32 r) 
            { 
                if (r != mr)
                {
                    mr = r; 
                    mmask.clear(); 
                    mmask.resize((2 * mr + 1) * (2 * mr + 1));

                    for (auto& e : mmask)
                    {
                        e = 0;
                    }
                    fill_mask();
                }
            }

            inline u32 get_r() const {return mr;}
            inline void check_alpha() {mfix_alpha = true;}
            inline void no_check_alpha() {mfix_alpha = false;}
            inline void toggle_check_alpha() {mfix_alpha = !mfix_alpha;}
            inline b8 checks_alpha() const {return mfix_alpha;}
            virtual inline ToolType get_type() const override {return ToolType::Brush;}

            virtual void draw(Canvas* c) override;
            virtual void no_alpha_fix_draw(Canvas* c);
            virtual void alpha_fix_draw(Canvas* c);
        protected:
            u32 mr = 5;
            std::vector<u32> mmask;
            b8 mfix_alpha = true;
            b8 mfilled = false;
    };
    class SimpleBrush : public Brush
    {
        public:
            SimpleBrush(i64 x = 0, i64 y = 0, u32 r = 5);
            inline void set_colour(u32 col) 
            {
                if (col == mcolour) return;
                mcolour = col;
                if (mfilled)
                {
                    zero_mask();
                    fill_mask();
                }
            }
            inline u32 get_colour() const {return mcolour;}
            virtual void fill_mask() override;
            inline virtual void zero_mask() override {mmask.assign(mmask.size(), 0);}
        protected:
            u32 mcolour;
    };
    class LinearBrush : public SimpleBrush 
    {
        public:
            LinearBrush(i64 x =0, i64 y = 0, u32 r = 5): SimpleBrush(x, y, r){zero_mask(); fill_mask();}
            virtual void fill_mask() override;
    };
    class QuadraticBrush : public SimpleBrush
    {
        public:
            QuadraticBrush(i64 x =0, i64 y = 0, u32 r = 5): SimpleBrush(x, y, r){zero_mask(); fill_mask();}
            virtual void fill_mask() override;
    };

    class SprayBrush : public SimpleBrush
    {
        public:
            SprayBrush(i64 x =0, i64 y = 0, u32 r = 5): SimpleBrush(x, y, r), mchances(40){zero_mask(); fill_mask();}
            virtual void fill_mask() override;
            void set_chances(u8 chance);
            inline u8 get_chances()  const {return mchances;}
        private:
            u8 mchances;
    };
    class SmudgeBrush : public Brush
    {
        public:
            SmudgeBrush(i64 x =0, i64 y = 0, u32 r = 5);
            virtual inline void fill_mask() override{mfilled = true;}
            inline virtual void zero_mask() override {mmask.assign(mmask.size(), 0);}
            virtual void draw(Canvas* c) override;
    };

    class SpeedBrush : public SimpleBrush
    {
        public:
            SpeedBrush(i64 x =0, i64 y = 0, u32 r = 5): SimpleBrush(x, y, r),
            mmax_r(20), mmin_r(1){mprev_x = x; mprev_y = y; zero_mask(); fill_mask();}
            virtual void draw(Canvas* c) override;
            inline void set_prev_x(i64 x) {mprev_x = x;}
            inline void set_prev_y(i64 y) {mprev_y = y;}
            inline void set_min_r(u32 r) {mmin_r = r;}
            inline void set_max_r(u32 r) {mmax_r = r;}
        private:
            i64 mprev_x;
            i64 mprev_y;
            u32 mmax_r;
            u32 mmin_r;
    };

}
#endif
