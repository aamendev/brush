#ifndef WINDOW_H
#define WINDOW_H
#include <string>
#include <wayland-client-protocol.h>
#include "core/cursor.h"
#include "events/event_handler.h"
#include "types.h"
#include "xdg-shell-proto.h"
#include "internal.h"
struct WindowSettings
{
    i32 width = 600;
    i32 height = 600;
    std::string title = "";
    u32* pixels = nullptr;
};
struct WaylandWindowSettings
{
    wl_callback_listener surface_frame_listener;
    struct wl_surface* surface;
    struct xdg_surface* xsurface;
    struct xdg_toplevel* xtoplevel;
    struct wl_display* display;
    u32 last_frame;
    struct wl_shm* shm;
    struct wl_registry* reg;
    struct wl_compositor* compositor;
    struct xdg_wm_base* xwm_base;
};
class Window 
{
    public:
        Window() = default;
        Window(WindowSettings&& ws): msettings(ws){};
        Window(const WindowSettings& ws){Window(std::move(ws));};

        b8 update();
        void init();
        void wclose();
        void request_event_type(EventType&& ev);
        template <typename ET, typename ... Args, typename ... fArgs>
            inline void request_subscribe(std::function<void(Args...)> f, fArgs&... args)
            {
                mevent_handler.request_subscribe<ET>(f, args...);
            }
        void fill(u32 col);
        void fill(u32* col, u32 x, u32 y, u32 w, u32 h);
        void outline_cursor(Core::Cursor* curs);
       inline void handle_events() {mevent_handler.handle_events();}
    public: // setters
        inline void set_width(i32 w) {msettings.width = w;}
        inline void set_height(i32 h) {msettings.height = h;}
        inline void set_title(std::string&& s) {msettings.title = s;}
        inline void set_settings(WindowSettings&& ws){msettings = ws;}
    public: // getters
        inline i32 get_width() const {return msettings.width;}
        inline i32 get_height() const {return msettings.height;}
        inline const std::string& get_title() const {return msettings.title;}
    private: // wayland configs
        static void wl_buffer_release(void* data, struct wl_buffer* buf);
        static void frame_done(void* data, struct wl_callback* cb, u32 time);
        static void xdg_surface_config(void* data, 
                struct xdg_surface *xsurface, u32 serial);

        static void
            xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, u32 serial);

        static struct wl_buffer* draw_frame(Window*);

        static void reg_global_handler(void* data, struct wl_registry* reg, 
                u32 name, const char* interface, 
                u32 ver);
        static void reg_global_remove_handler(void* data, struct wl_registry* reg,
                u32 name);

        const struct wl_buffer_listener wl_buffer_listener
        {
            .release = Window::wl_buffer_release
        };
        const struct wl_registry_listener reg_listener = 
        {
            .global = Window::reg_global_handler,
            .global_remove = Window::reg_global_remove_handler
        };
        const struct xdg_surface_listener xdg_surface_listener = 
        {
            .configure = Window::xdg_surface_config
        };
        const struct xdg_wm_base_listener xdg_wm_base_listener = {
            .ping = Window::xdg_wm_base_ping,
        };
    private: // accessible
        WindowSettings msettings;
        b8 running;
        EventHandler mevent_handler;
    private: // non-accessible
        WaylandWindowSettings mwayland_settings;
};

#endif
