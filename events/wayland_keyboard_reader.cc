#include "wayland_keyboard_reader.h"
#include "keyboard.h"
#include <complex.h>
#include <unistd.h>
#include <wayland-client-protocol.h>
#include <wayland-util.h>
#include <xkbcommon/xkbcommon.h>

WaylandKeyboardReader::WaylandKeyboardReader()
{
    mlistener.enter = on_enter;
    mlistener.leave = on_leave;

    mlistener.key = on_key;
    mlistener.keymap = on_keymap;

    mlistener.modifiers = on_mods;
    mlistener.repeat_info = on_repeat;

    mxcontext = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
}
void WaylandKeyboardReader::on_keymap(void* data, struct wl_keyboard * kb, u32 format, i32 fd, u32 size)
{
    WaylandKeyboardReader* ke = (WaylandKeyboardReader*)data;

    char* map_shm = (char*)mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);

    struct xkb_keymap * xkm = xkb_keymap_new_from_string(
            ke->mxcontext, map_shm, 
            XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS
            );
    munmap(map_shm, size);
    close(fd);

    struct xkb_state* xstate = xkb_state_new(xkm);
    xkb_keymap_unref(ke->mxkm);
    xkb_state_unref(ke->mxstate);
    ke->mxkm = xkm;
    ke->mxstate = xstate;
}

void WaylandKeyboardReader::on_enter(void* data, struct wl_keyboard* kb, u32 serial,
        struct wl_surface* surface, struct wl_array* keys)
{
    WaylandKeyboardReader* ke = (WaylandKeyboardReader*)data;
    u32* key;

    for (key = (u32*)(keys)->data; 
            (keys)->size != 0 && 
            (const char *) key < ((const char *) (keys)->data + (keys)->size);
            (key)++)
    {
        char buf[128];
        xkb_keysym_t sym = xkb_state_key_get_one_sym(
                ke->mxstate, *key + 8
                );
        xkb_keysym_get_name(sym, buf, sizeof(buf));
        xkb_state_key_get_utf8(ke->mxstate, *key + 8, 
                buf, sizeof(buf));
    }
}

void WaylandKeyboardReader::on_leave(void* data, struct wl_keyboard* kb, 
        u32 serial, struct wl_surface* s)
{
}

void WaylandKeyboardReader::on_key(void* data, struct wl_keyboard* kb, u32 serial,
        u32 time, u32 key, u32 state)
{
    WaylandKeyboardReader* ke = (WaylandKeyboardReader*)data;
    char buf[128];
    u32 keycode = key + 8;
    xkb_keysym_t sym = xkb_state_key_get_one_sym(
            ke->mxstate, keycode
            );
    xkb_keysym_get_name(sym, buf, sizeof(buf));
    if (state == WL_KEYBOARD_KEY_STATE_PRESSED)
    {
        auto* kb_event = new Events::Keyboard::KeyPress;
        kb_event->mkey = (Events::Keyboard::Key)buf[0];
        ke->mevents.push_back(kb_event);
    }
    else 
    {
        auto* kb_event = new Events::Keyboard::KeyRelease;
        kb_event->mkey = (Events::Keyboard::Key)buf[0];
        ke->mevents.push_back(kb_event);
    }

    ke->mcurrkey = (Events::Keyboard::Key)buf[0];

    xkb_state_key_get_utf8(
            ke->mxstate, keycode, buf, sizeof(buf)
            );
}

void WaylandKeyboardReader::on_mods(void* data, struct wl_keyboard* kb,
        u32 serial, u32 mods_depress, u32 mods_latch, u32 mods_lock, u32 group)
{
    WaylandKeyboardReader* ke = (WaylandKeyboardReader*) data;
    xkb_state_update_mask(
            ke->mxstate, 
            mods_depress, mods_latch, mods_lock,
            0, 0, group);
}

void WaylandKeyboardReader::on_repeat(void* data, struct wl_keyboard* kb,
        i32 rate, i32 delay)
{

    WaylandKeyboardReader* ke = (WaylandKeyboardReader*)data;
    auto* kb_event = new Events::Keyboard::KeyPress;
    kb_event->mkey = ke->mcurrkey;
    ke->mevents.push_back(kb_event);
}
