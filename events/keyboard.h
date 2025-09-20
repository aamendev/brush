#ifndef EVENT_KEYBOARD_H
#define EVENT_KEYBOARD_H
#include <iostream>
#include "../types.h"
#include "event.h"
namespace Events { namespace Keyboard {
    enum class Key
    {
        A = 'A', a = 'a',
        B = 'B', b = 'b',
        C = 'C', c = 'c',
        D = 'D', d = 'd',
        E = 'E', e = 'e',
        F = 'F', f = 'f',
        G = 'G', g = 'g',
        H = 'H', h = 'h',
        I = 'I', i = 'i',
        J = 'J', j = 'j',
        K = 'K', k = 'k',
        L = 'L', l = 'l',
        M = 'M', m = 'm',
        N = 'N', n = 'n',
        O = 'O', o = 'o',
        P = 'P', p = 'p',
        Q = 'Q', q = 'q',
        R = 'R', r = 'r',
        S = 'S', s = 's',
        T = 'T', t = 't',
        U = 'U', u = 'u',
        V = 'V', v = 'v',
        W = 'W', w = 'w',
        X = 'X', x = 'x',
        Y = 'Y', y = 'y',
        Z = 'Z', z = 'z',
        Zero = '0',
        One = '1',
        Two = '2',
        Three = '3',
        Four = '4',
        Five = '5',
        Six = '6',
        Seven = '7',
        Eight = '8',
        Nine = '9',
        LeftBracket = '[', RightBracket = ']',
    };
    class KeyPress : public Event
    {
        public:
            friend class WaylandKeyboardReader;
            friend class EventHandler;
            inline Key get_key() const {return mkey;}
            inline static Type get_static_type() {return Type::KeyPress;}
            inline virtual Type getType() const {return get_static_type();};
            Key mkey;
    };

    class KeyRelease : public Event
    {
        public:
            friend class WaylandKeyboardReader;
            friend class EventHandler;
            inline Key get_key() const {return mkey;}
            inline static Type get_static_type() {return Type::KeyRelease;}
            inline virtual Type getType() const {return get_static_type();};
            Key mkey;
    };
}}
#endif
