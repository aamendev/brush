#ifndef WKB_READER_H
#define WKB_READER_H

#include <iostream>
#include <cstring>
#include "../types.h"
#include "event.h"
#include "keyboard.h"
#include <wayland-client-protocol.h>
#include <wayland-util.h>
#include <sys/mman.h>
#include <xkbcommon/xkbcommon.h>


class WaylandKeyboardReader{
    public:
    friend class EventHandler;

    WaylandKeyboardReader();

    static void on_keymap(void* data, struct wl_keyboard * kb, u32 format, i32 fd, u32 size);

    static void on_enter(void* data, struct wl_keyboard* kb, u32 serial,
            struct wl_surface* surface, struct wl_array* keys);

    static void on_key(void* data, struct wl_keyboard* kb, u32 serial,
            u32 time, u32 key, u32 state);

    static void on_leave(void* data, struct wl_keyboard* kb, 
            u32 serial, struct wl_surface* s);

    static void on_mods(void* data, struct wl_keyboard* kb,
            u32 serial, u32 mods_depress, u32 mods_latch, u32 mods_lock, u32 group);
    static void on_repeat(void* data, struct wl_keyboard* kb,
            i32 rate, i32 delay);

    private:
    inline struct wl_keyboard* pget_kboard() {return kboard;}
    inline void set_kboard(struct wl_keyboard* kb) {kboard = kb;}
    inline wl_keyboard_listener* pget_listener() {return &mlistener;}
    

    private:
    struct wl_keyboard_listener mlistener;
    struct wl_keyboard* kboard;
    struct xkb_context* mxcontext;
    struct xkb_keymap* mxkm = NULL;
    struct xkb_state* mxstate = NULL;
    Events::Keyboard::Key mcurrkey;
    std::vector<Events::Event*> mevents;
};

#endif
