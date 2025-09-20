#ifndef EVENT_MOUSE_H
#define EVENT_MOUSE_H
#include <iostream>
#include "../types.h"
#include "event.h"
namespace Events { namespace Mouse {
    enum class Button
    {
       Left = 272,
       Right,
       None,
    };
    class ButtonPress : public Event
    {
        public:
            friend class WaylandPointerReader;
            friend class EventHandler;

            inline Button get_button() const {return mbutton;}
            inline static Type get_static_type() {return Type::MouseButtonPress;}
            inline virtual Type getType() const {return get_static_type();};
            Button mbutton;
    };

    class ButtonRelease : public Event
    {
        public:
            friend class WaylandKeyboardReader;
            friend class EventHandler;
            inline Button get_button() const {return mbutton;}
            inline static Type get_static_type() {return Type::MouseButtonRelease;}
            inline virtual Type getType() const {return get_static_type();};
            Button mbutton;
    };

    class Move : public Event
    {
        public:
            friend class WaylandKeyboardReader;
            friend class EventHandler;
            inline f32 get_x() const {return mx;}
            inline f32 get_y() const {return my;}
            inline static Type get_static_type() {return Type::MouseMove;}
            inline virtual Type getType() const {return get_static_type();};
            f32 mx;
            f32 my;
    };
    class Enter : public Event
    {
        public:
            inline static Type get_static_type() {return Type::MouseEnter;}
            inline virtual Type getType() const {return get_static_type();}
    };

    class Leave : public Event
    {
        public:
            inline static Type get_static_type() {return Type::MouseLeave;}
            inline virtual Type getType() const {return get_static_type();}
    };
}}
#endif
