#ifndef EVENTS_EVENT_HANDLER_H
#define EVENTS_EVENT_HANDLER_H
#include "wayland_keyboard_reader.h"
#include "wayland_pointer_reader.h"
#include <functional>
#include <wayland-client-protocol.h>
#include <vector>
enum class EventType
{
    Keyboard,
    Pointer,
};
class EventHandler 
{
    public:
        EventHandler();

        inline void request_event_type(EventType et) { mrequested_events.push_back(et);}
        template <typename ET, typename ... Args, typename ... fArgs>
            void request_subscribe(std::function<void(Args...)> f, fArgs&... args)
            {
                for (auto* e : mkb_event.mevents)
                {
                    if (e->getType() == ET::get_static_type())
                    {
                        e->subscribe<ET>(f, args...);
                    }
                }

                for (auto* e : mp_event.mevents)
                {
                    if (e->getType() == ET::get_static_type())
                    {
                        e->subscribe<ET>(f, args...);
                    }
                }
            }

        void handle_events();
    public:
        struct wl_seat* pget_seat() {return mseat;}
        struct wl_seat_listener* pget_listener() {return &mlistener;}
        void set_seat(struct wl_seat* s) {mseat = s;}
    private:
        void register_event_type(EventType);
        void register_subscribe(EventType);
        static void wl_seat_name(void* data, struct wl_seat* wl_seat, const char* name);
        static void wl_seat_capabilites(void* data, struct wl_seat* seat, uint32_t cap);
    private:
        WaylandKeyboardReader mkb_event;
        WaylandPointerReader mp_event;
        struct wl_seat* mseat;
        u32 mcapabilities;
        std::vector<EventType> mrequested_events;
        struct wl_seat_listener mlistener;
};
#endif
