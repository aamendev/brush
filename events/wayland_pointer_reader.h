#ifndef WP_READER_H
#define WP_READER_H
#include <iostream>
#include <cstring>
#include "../types.h"
#include <functional>
#include "event.h"
#include <wayland-client-protocol.h>
#include <wayland-util.h>

enum WaylandPointerReaderMask {
    POINTER_EVENT_ENTER = 1 << 0,
    POINTER_EVENT_LEAVE = 1 << 1,
    POINTER_EVENT_MOTION = 1 << 2,
    POINTER_EVENT_BUTTON = 1 << 3,
    POINTER_EVENT_AXIS = 1 << 4,
    POINTER_EVENT_AXIS_SOURCE = 1 << 5,
    POINTER_EVENT_AXIS_STOP = 1 << 6,
    POINTER_EVENT_AXIS_DISCRETE = 1 << 7,
};


class WaylandPointerReader{
    public:
        friend class EventHandler;
    WaylandPointerReader();

    inline void set_on_enter(std::function<void(void* data)> f){on_enter = f;}
    inline void set_on_leave(std::function<void(void* data)> f){on_leave = f;}
    inline void set_on_button(std::function<void(void* data)> f){on_button = f;}
    inline void set_on_axis(std::function<void(void* data)> f){on_axis = f;}
    inline void set_on_motion(std::function<void(void* data)> f){on_motion = f;}
    inline void set_on_stop(std::function<void(void* data)> f){on_stop = f;}

    static void static_on_enter(void* data, struct wl_pointer* pointer,
            u32 serial, struct wl_surface* s,
            wl_fixed_t s_x, wl_fixed_t s_y);

    static void static_on_leave(void* data, struct wl_pointer* pointer,
            u32 serial, struct wl_surface* s);

    static void static_on_motion(void* data, struct wl_pointer* pointer,
            u32 time, wl_fixed_t s_x, wl_fixed_t s_y);

    static void static_on_button(void* data, struct wl_pointer* pointer,
            u32 serial, u32 time,
            u32 buttstatic_on, u32 bstate);

    static void static_on_axis(void* data, struct wl_pointer* pointer,
            u32 time, u32 axis, wl_fixed_t value);

    static void static_on_source(void* data, struct wl_pointer* pointer,
            u32 axis_source);

    static void static_on_stop(void* data, struct wl_pointer* pointer,
            u32 time, u32 axis);

    static void static_on_discrete(void* data, struct wl_pointer* pointer,
            u32 axis, i32 discrete);

    static void event_frame(void *data, wl_pointer* pl);

    void set_pointer_listener(struct wl_pointer_listener* pl);
    void clear();

    inline struct wl_pointer_listener* pget_listener() {return &pointer_listener;}
    inline void set_pointer(struct wl_pointer* p) {pointer = p;}
    inline struct wl_pointer* pget_pointer() {return pointer;}

    private:
    struct wl_pointer_listener pointer_listener;
    struct wl_pointer* pointer;
    u32 mevent_mask;
    wl_fixed_t msx;
    wl_fixed_t msy;
    u32 mbutton, mstate;
    u32 mtime;
    u32 mserial;
    struct wl_surface* msurface;
    std::function<void(void* data)> on_enter;
    std::function<void(void* data)> on_leave;
    std::function<void(void* data)> on_axis;
    std::function<void(void* data)> on_button;
    std::function<void(void* data)> on_motion;
    std::function<void(void* data)> on_stop;
    struct {
        int valid;
        wl_fixed_t value;
        i32 discrete;
    } maxes[2];
    u32 maxis_source;

    std::vector<Events::Event*> mevents;
};

#endif
