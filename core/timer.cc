#include "timer.h"
#include <chrono>

namespace Core 
{

    void Timer::wait(f32 time)
    {
        if (time < 0) return;
        auto initTime = std::chrono::high_resolution_clock::now();
        auto currentTime = initTime;
        do 
        {
            currentTime = std::chrono::high_resolution_clock::now();
            mwait_time = currentTime - initTime; 
        } while ((mwait_time).count() * 1000 < time);
    }

};
