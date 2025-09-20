#ifndef TIMER_H
#define TIMER_H
#include "../types.h"
#include <chrono>
namespace Core {
        enum class TimeUnit
    {
        Micro = (u32)1e6,
        Mili = (u32)1e3,
        Sec,
        Min,
        Hour,
    };

    class Timer 
    {
        public:
        inline void begin() {mstart = std::chrono::steady_clock::now();}
        inline void end() {mend = std::chrono::steady_clock::now();};
        inline f32 get_time(TimeUnit u = TimeUnit::Mili) {
            return 
                (static_cast<std::chrono::duration<f32>>((mend - mstart))).count() * 1000;};
        void wait(f32 time);
        private:
        std::chrono::time_point<std::chrono::steady_clock> mstart;
        std::chrono::time_point<std::chrono::steady_clock> mend;
        std::chrono::duration<f32> mwait_time;

    };
}
#endif
