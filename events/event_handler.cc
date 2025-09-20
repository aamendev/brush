#include "event_handler.h"

EventHandler::EventHandler() : mcapabilities(0)
{
    mlistener.capabilities = wl_seat_capabilites;
    mlistener.name = wl_seat_name;
}

void EventHandler::wl_seat_name(void* data, struct wl_seat* wl_seat, const char* name)
{
}

void EventHandler::wl_seat_capabilites(void* data, struct wl_seat* seat, uint32_t cap)
{
    EventHandler* eh = (EventHandler*)data;

    if (!(cap & WL_SEAT_CAPABILITY_POINTER) && eh->mp_event.pget_pointer() != NULL)
    {
        wl_pointer_release(eh->mp_event.pget_pointer());
        eh->mp_event.set_pointer(NULL);
    }
    if (!(cap & WL_SEAT_CAPABILITY_KEYBOARD) && eh->mkb_event.pget_kboard() != NULL)
    {
        wl_keyboard_release(eh->mkb_event.pget_kboard());
        eh->mkb_event.set_kboard(NULL);
    }
    eh->mcapabilities = cap;
    for (auto et : eh->mrequested_events)
    {
        eh->register_event_type(et);
    }
}
void EventHandler::register_event_type(EventType event_type)
{
    switch(event_type)
    {
        case EventType::Keyboard:
            {
                if(mcapabilities & WL_SEAT_CAPABILITY_KEYBOARD)
                {
                    mkb_event.set_kboard(wl_seat_get_keyboard(mseat));
                    wl_keyboard_add_listener(mkb_event.pget_kboard(),
                            mkb_event.pget_listener(), &mkb_event);
                }
            }
            break;
        case EventType::Pointer:
            {
                if(mcapabilities & WL_SEAT_CAPABILITY_POINTER)
                {

                    mp_event.set_pointer(wl_seat_get_pointer(mseat));
                    wl_pointer_add_listener(mp_event.pget_pointer(),
                            mp_event.pget_listener(), &mp_event);
                }
            }
            break;

    };
}

void EventHandler::handle_events()
{
    for (auto* e : mkb_event.mevents)
    {
        e->handle(); 
        delete e;
    }
    mkb_event.mevents.clear();

    for (auto* e : mp_event.mevents)
    {
        e->handle();
        delete e;
    }
    mp_event.mevents.clear();
}
