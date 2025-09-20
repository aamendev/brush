#ifndef EVENT_H
#define EVENT_H
#include <functional>
#include "../types.h"
#include <iostream>
namespace Events {
    enum class Type
    {
        KeyPress, KeyRelease, 
        MouseMove, MouseButtonPress, MouseButtonRelease, MouseEnter, MouseLeave,
        WindowResize, WindowClose,
        None,
    };
    class Event
    {
        public:
            virtual ~Event() = default;
            virtual Type getType() const {return Type::None;};
            inline void keep() {mkeep = true;}
            inline void nokeep() {mkeep = true;}

            template<typename ET, typename ...Args, typename ...fArgs>
                void subscribe(std::function<void(Args...)>f, fArgs&... args) 
                {
                    std::function<void()> f2 = std::bind(f, dynamic_cast<ET*>(this), std::ref(args)...);
                    msubscribers.push_back(f2);
                }
            b8 handle()
            {
                for (auto f : msubscribers)
                {
                    f();
                }
                msubscribers.clear();
                return true;
            }

        private:
            std::vector<std::function<void()>> msubscribers;
            b8 mkeep;

    };
}
#endif
