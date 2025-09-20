#ifndef CURSOR_H
#define CURSOR_H
#include "../types.h"
#include "../graphics/canvas.h"
#include "../graphics/colours.h"
namespace Core{
    class Cursor
    {
        public:
            Cursor(i64 x = 0, i64 y = 0, u32 r = 5);
            //void fill_cursor();
            inline void set_x(u32 x) {mx = x;}
            inline void set_y(u32 y) {my = y;}
            inline u32 get_x() const {return mx;}
            inline u32 get_y() const {return my;}
            inline u32 get_r() const {return mr;}
        public:
            u32* curs_pixels;
        private:
            i64 mx;
            i64 my;
            u32 mr;
            u32 mcolour;
    };
}
#endif
