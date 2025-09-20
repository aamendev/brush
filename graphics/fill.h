#ifndef FILL_H
#define FILL_H
#include "../types.h"
#include "canvas.h"
#include <math.h>
#include "colours.h"
#include "tool.h"
namespace Graphics{
    class Fill : public Tool
    {
        public:
            Fill(i64 x = 0, i64 y = 0);
            virtual void draw(Canvas* c) override;
            inline void set_colour(u32 col) {mcolour = col;}
            inline u32 get_colour() const {return mcolour;}
            virtual inline ToolType get_type() const override {return ToolType::Fill;}
        private:
            u32 mcolour;
    };
}
#endif

