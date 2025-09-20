#ifndef TOOL_H
#define TOOL_H
#include "canvas.h"
#include "../core/utils.h"
namespace Graphics{
    enum class ToolType
    {
        Brush,
        Fill,
        None
    };
    class Tool 
    {
        public:
            virtual void draw(Canvas* c) = 0;
            inline void set_x(i64 x) {mx = x;}
            inline u32 get_x() const {return mx;}

            inline void set_y(i64 y) {my = y;}
            inline u32 get_y() const {return my;}
            inline virtual ToolType get_type() const = 0;
        protected:
            i64 mx;
            i64 my;
    };
}
#endif
