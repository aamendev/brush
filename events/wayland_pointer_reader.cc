#include "wayland_pointer_reader.h"
#include "mouse.h"
#include <wayland-client-protocol.h>
#include <wayland-util.h>
WaylandPointerReader::WaylandPointerReader()
{
    clear();
    pointer_listener.enter = static_on_enter;
    pointer_listener.leave = static_on_leave;
    pointer_listener.motion = static_on_motion;
    pointer_listener.button = static_on_button;
    pointer_listener.axis = static_on_axis;
    pointer_listener.frame = event_frame;
    pointer_listener.axis_source = static_on_source;
    pointer_listener.axis_stop = static_on_stop;
    pointer_listener.axis_discrete = static_on_discrete;
}

void WaylandPointerReader::clear()
{
    mevent_mask = 0; 
    msx = 0;
    msy = 0;
    mbutton = 0;
    mstate = 0;
    mtime = 0;
    mserial = 0;
    maxes[0] = {0, 0 ,0};
    maxes[1] = {0, 0, 0};
    maxis_source = 0;
}
void WaylandPointerReader::event_frame(void *data, wl_pointer* pl)
{
    WaylandPointerReader* pe = (WaylandPointerReader*)data;
    (void)pe;


    if (pe->mevent_mask & WaylandPointerReaderMask::POINTER_EVENT_ENTER) 
    {
        wl_pointer_set_cursor(pl, pe->mserial, NULL, wl_fixed_to_int(pe->msx), 
                wl_fixed_to_int(pe->msy));
        auto* me = new Events::Mouse::Enter;
        pe->mevents.push_back(me);
    }



    if (pe->mevent_mask & WaylandPointerReaderMask::POINTER_EVENT_LEAVE) 
    {

        auto* ml = new Events::Mouse::Leave;
        pe->mevents.push_back(ml);
    }
    if (pe->mevent_mask & WaylandPointerReaderMask::POINTER_EVENT_MOTION) {
        /*fprintf(stderr, "motion %f, %f ",
          wl_fixed_to_double(pe->msx),
          wl_fixed_to_double(pe->msy));
          */
        auto* mv = new Events::Mouse::Move;
        mv->mx = (u32)wl_fixed_to_int(pe->msx);
        mv->my = (u32)wl_fixed_to_int(pe->msy);
        pe->mevents.push_back(mv);
    }

    if (pe->mevent_mask & WaylandPointerReaderMask::POINTER_EVENT_BUTTON) {
        if(pe->mstate == WL_POINTER_BUTTON_STATE_PRESSED)
        {
            auto* mv = new Events::Mouse::ButtonPress;
            mv->mbutton = (Events::Mouse::Button)pe->mbutton;
            pe->mevents.push_back(mv);
        }
        else 
        {
            auto* mv = new Events::Mouse::ButtonRelease;
            mv->mbutton = (Events::Mouse::Button)pe->mbutton;
            pe->mevents.push_back(mv);
        }
    }
    /*

       u32 axis_events = WaylandPointerReaderMask::POINTER_EVENT_AXIS
       | WaylandPointerReaderMask::POINTER_EVENT_AXIS_SOURCE
       | WaylandPointerReaderMask::POINTER_EVENT_AXIS_STOP
       | WaylandPointerReaderMask::POINTER_EVENT_AXIS_DISCRETE;
       const char *axis_name[2];
       axis_name[WL_POINTER_AXIS_VERTICAL_SCROLL] = "vertical";
       axis_name[WL_POINTER_AXIS_HORIZONTAL_SCROLL] = "horizontal";
       const char *axis_source[4]; 
       axis_source[WL_POINTER_AXIS_SOURCE_WHEEL] = "wheel";
       axis_source[WL_POINTER_AXIS_SOURCE_FINGER] = "finger";
       axis_source[WL_POINTER_AXIS_SOURCE_CONTINUOUS] = "continuous";
       axis_source[WL_POINTER_AXIS_SOURCE_WHEEL_TILT] = "wheel tilt";
       if (pe->mevent_mask & axis_events) {
       for (size_t i = 0; i < 2; ++i) {
       if (!pe->maxes[i].valid) {
       continue;
       }
       fprintf(stderr, "%s axis ", axis_name[i]);
       if (pe->mevent_mask & WaylandPointerReaderMask::POINTER_EVENT_AXIS) {
       fprintf(stderr, "value %f ", wl_fixed_to_double(
       pe->maxes[i].value));
       }
       if (pe->mevent_mask & WaylandPointerReaderMask::POINTER_EVENT_AXIS_DISCRETE) {
       fprintf(stderr, "discrete %d ",
       pe->maxes[i].discrete);
       }
       if (pe->mevent_mask & WaylandPointerReaderMask::POINTER_EVENT_AXIS_SOURCE) {
       std::cerr << "src: " << pe->maxis_source << '\n';
       fprintf(stderr, "via %s ",
       axis_source[(i64)pe->maxis_source]);
       (void)axis_source;
       }
       if (pe->mevent_mask & WaylandPointerReaderMask::POINTER_EVENT_AXIS_STOP) {
       fprintf(stderr, "(stopped) ");
       }
       }
       }

       std::cerr << '\n';
       */
    pe->clear();
}
void WaylandPointerReader::static_on_enter(void* data, struct wl_pointer* pointer,
        u32 serial, struct wl_surface* s,
        wl_fixed_t s_x, wl_fixed_t s_y)
{
    WaylandPointerReader* pe = (WaylandPointerReader*)data;
    pe->mevent_mask |= WaylandPointerReaderMask::POINTER_EVENT_ENTER;
    pe->mserial = serial;
    pe->msurface = s;
    pe->msx = s_x;
    pe->msy = s_y;
}
void WaylandPointerReader::static_on_leave(void* data, struct wl_pointer* pointer,
        u32 serial, struct wl_surface* s)
{
    WaylandPointerReader* pe = (WaylandPointerReader*)data;
    pe->mevent_mask |= WaylandPointerReaderMask::POINTER_EVENT_LEAVE;
    pe->mserial = serial;
}
void WaylandPointerReader::static_on_motion(void* data, struct wl_pointer* pointer,
        u32 time, wl_fixed_t s_x, wl_fixed_t s_y)
{

    WaylandPointerReader* pe = (WaylandPointerReader*)data;
    pe->mevent_mask |= WaylandPointerReaderMask::POINTER_EVENT_MOTION;
    pe->mtime = time;
    pe->msx = s_x;
    pe->msy = s_y;

}
void WaylandPointerReader::static_on_button(void* data, struct wl_pointer* pointer,
        u32 serial, u32 time,
        u32 button, u32 bstate)
{
    WaylandPointerReader* pe = (WaylandPointerReader*)data;
    pe->mevent_mask |= WaylandPointerReaderMask::POINTER_EVENT_BUTTON;
    pe->mtime = time;
    pe->mserial = serial;
    pe->mbutton = button;
    pe->mstate = bstate;

}
void WaylandPointerReader::static_on_axis(void* data, struct wl_pointer* pointer,
        u32 time, u32 axis, wl_fixed_t value)
{
    WaylandPointerReader* pe = (WaylandPointerReader*)data;
    pe->mevent_mask |= WaylandPointerReaderMask::POINTER_EVENT_AXIS;
    pe->mtime = time;
    pe->maxes[axis].valid = 1;
    pe->maxes[axis].value = value;

}
void WaylandPointerReader::static_on_source(void* data, struct wl_pointer* pointer,
        u32 axis_source)
{

    WaylandPointerReader* pe = (WaylandPointerReader*)data;
    pe->mevent_mask |= WaylandPointerReaderMask::POINTER_EVENT_AXIS_SOURCE;
    pe->maxis_source = axis_source;

}
void WaylandPointerReader::static_on_stop(void* data, struct wl_pointer* pointer,
        u32 time, u32 axis)
{

    WaylandPointerReader* pe = (WaylandPointerReader*)data;
    pe->mevent_mask |= WaylandPointerReaderMask::POINTER_EVENT_AXIS_STOP;
    pe->mtime = time;
    pe->maxes[axis].valid = 1;
}
void WaylandPointerReader::static_on_discrete(void* data, struct wl_pointer* pointer,
        u32 axis, i32 discrete)
{
    WaylandPointerReader* pe = (WaylandPointerReader*)data;
    pe->mevent_mask |= WaylandPointerReaderMask::POINTER_EVENT_AXIS_DISCRETE;
    pe->maxes[axis].valid = 1;
    pe->maxes[axis].discrete = discrete;

}
