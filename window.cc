#include  "window.h"
#include "core/utils.h"
#include "graphics/canvas.h"
#include "graphics/shapes.h"

void Window::init()
{
    msettings.pixels = new u32[msettings.width * msettings.height];
    mwayland_settings.surface_frame_listener = 
    {
        .done = Window::frame_done
    };


    mwayland_settings.display = wl_display_connect(NULL);
    mwayland_settings.reg = wl_display_get_registry(
            mwayland_settings.display);

    if (!mwayland_settings.display)
    {
        fprintf(stderr, "Failed to connect to server\n");
        exit(1);
    }

    wl_registry_add_listener(mwayland_settings.reg,
            &reg_listener, this);
    wl_display_roundtrip(mwayland_settings.display);

    mwayland_settings.surface = wl_compositor_create_surface(
            mwayland_settings.compositor);
   // state.subsurface = wl_subcompositor_get_subsurface(state.subcompositor, state.subsurface, state.surface);

    mwayland_settings.xsurface = xdg_wm_base_get_xdg_surface(mwayland_settings.xwm_base, mwayland_settings.surface);
    
    xdg_surface_add_listener(mwayland_settings.xsurface, &xdg_surface_listener, this);
    mwayland_settings.xtoplevel = xdg_surface_get_toplevel(mwayland_settings.xsurface);
    
    xdg_toplevel_set_title(mwayland_settings.xtoplevel, msettings.title.c_str());
    wl_surface_commit(mwayland_settings.surface);

    struct wl_callback* cb = wl_surface_frame(mwayland_settings.surface);
    wl_callback_add_listener(cb,
            &mwayland_settings.surface_frame_listener,
            this);
    
}

void Window::request_event_type(EventType&& ev)
{
    mevent_handler.request_event_type(ev);
}

b8 Window::update()
{
   return wl_display_dispatch(mwayland_settings.display);
}

void Window::wclose()
{
    wl_display_disconnect(mwayland_settings.display);
}

void Window::reg_global_handler(void* data, struct wl_registry* reg, 
        u32 name, const char* interface, 
        u32 ver)
{

    Window* win = (Window*)data;

    if (strcmp(interface, wl_compositor_interface.name) == 0)
    {
        win->mwayland_settings.compositor = (struct wl_compositor*)wl_registry_bind(
                reg, 
                name, 
                &wl_compositor_interface, 4);
    }

    else if (strcmp(interface, wl_seat_interface.name) == 0)
    {
        win->mevent_handler.set_seat((struct wl_seat*)wl_registry_bind(
                    reg, 
                    name, 
                    &wl_seat_interface, 7));
        wl_seat_add_listener(win->mevent_handler.pget_seat(), 
                win->mevent_handler.pget_listener(), 
                &win->mevent_handler);
    }

    else if (strcmp(interface, wl_shm_interface.name) == 0)
    {
        win->mwayland_settings.shm = (struct wl_shm*) wl_registry_bind(
                reg,
                name,
                &wl_shm_interface,
                1
                );
    }
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
    {
        win->mwayland_settings.xwm_base = (struct xdg_wm_base*) wl_registry_bind(
                reg,
                name,
                &xdg_wm_base_interface,
                1
                );
        xdg_wm_base_add_listener(win->mwayland_settings.xwm_base, &win->xdg_wm_base_listener, 
                win);
    }
}

void Window::reg_global_remove_handler(void* data, struct wl_registry* reg,
        u32 name)
{

}
void Window::wl_buffer_release(void* data, struct wl_buffer* buf)
{
    wl_buffer_destroy(buf);
}
struct wl_buffer* Window::draw_frame(Window* ws)
{
    const int stride = ws->get_width() * 4;
    const int shm_pool_size = ws->get_height() * stride;


    int fd = allocate_shm_file(shm_pool_size);

    u8* pool_data = (u8*)mmap(NULL, shm_pool_size,
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    struct wl_shm_pool* buf_pool = wl_shm_create_pool(ws->mwayland_settings.shm, fd, shm_pool_size);

    int idx = 0;
    int offset = ws->get_height() * stride * idx;

    struct wl_buffer* front_buff = wl_shm_pool_create_buffer(
            buf_pool, offset, ws->get_width(), ws->get_height(), stride, WL_SHM_FORMAT_ARGB8888
            );
    wl_shm_pool_destroy(buf_pool);
    close(fd);

    uint32_t* pixels = (uint32_t*)&pool_data[0];
    for (int i = 0; i < ws->get_width() * ws->get_height(); i++)
    {
        pixels[i] = ws->msettings.pixels[i];
    }
    munmap(pool_data, shm_pool_size);
    wl_buffer_add_listener(front_buff, &ws->wl_buffer_listener, NULL);
    return front_buff;
}

void Window::fill(u32 col)
{
    for (int i = 0; i < get_width() * get_height(); i++)
    {
        msettings.pixels[i] = col;
    }
}

void Window::fill(u32* col, u32 x, u32 y, u32 w, u32 h)
{
    for (u32 j = 0; j < h; j++)
    {
        for (u32 i = 0; i < w; i++)
        {
            msettings.pixels[Core::Utils::wrap_around(y + j, 0, msettings.height) * 
                msettings.width + Core::Utils::wrap_around(x + i, 0, msettings.width)] 
                = col[j * w + i];
        }
    }
}
void Window::outline_cursor(Core::Cursor* curs)
{
    Graphics::Canvas c;
    c.pixels = msettings.pixels;
    c.width = msettings.width;
    c.height = msettings.height;
    Graphics::outline_circle(&c, curs->get_x(), curs->get_y(), curs->get_r(), 
            BLACK);
}
void Window::xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}
void Window::frame_done(void* data, struct wl_callback* cb, u32 time)
{

    Window* state = (Window*) data;
    wl_callback_destroy(cb);

    cb = wl_surface_frame(state->mwayland_settings.surface);
    wl_callback_add_listener(cb, &state->mwayland_settings.surface_frame_listener, data);

    // if (state->last_frame != 0)
    {
        int elapsed = time - state->mwayland_settings.last_frame;
        (void)elapsed;
        state->mwayland_settings.last_frame = time;
    }

    struct wl_buffer* buf = draw_frame(state);

    wl_surface_attach(state->mwayland_settings.surface, buf, 0, 0);
    wl_surface_damage_buffer(state->mwayland_settings.surface, 0, 0, INT32_MAX, INT32_MAX);
    wl_surface_commit(state->mwayland_settings.surface);
}

void Window::xdg_surface_config(void* data, 
        struct xdg_surface *xsurface, u32 serial)
{
    Window* state = (Window*)data;
    xdg_surface_ack_configure(xsurface, serial);
    struct wl_buffer* buf = draw_frame(state);

    wl_surface_attach(state->mwayland_settings.surface, buf, 0, 0);
    wl_surface_commit(state->mwayland_settings.surface);
}
