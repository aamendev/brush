#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <tuple>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-util.h>
#include <xkbcommon/xkbcommon.h>
#include "core/cursor.h"
#include "core/timer.h"
#include "core/utils.h"
#include "events/event_handler.h"
#include "events/keyboard.h"
#include "events/mouse.h"
#include "graphics/brush.h"
#include "graphics/colours.h"
#include "graphics/fill.h"
#include "graphics/shapes.h"
#include "graphics/tool.h"
#include "graphics/utils.h"
#include "window.h"
#include "graphics/canvas.h"
#include "graphics/canvas.h"
#include <queue>

#define WIDTH (600)
#define HEIGHT (600)

#define SIMPLE_IDX (0)
#define LINEAR_IDX (1)
#define QUADRATIC_IDX (2)
#define SMUDGE_IDX (3)
#define SPEED_IDX (4)
#define SPRAY_IDX (5)
#define FILL_IDX (6)

#define DEFAULT_R (20)

int draw_state = 0;


std::array<Graphics::Tool*, 7> tools{
    new Graphics::SimpleBrush(0, 0, DEFAULT_R),
            new Graphics::LinearBrush(0, 0, DEFAULT_R),
            new Graphics::QuadraticBrush(0, 0, DEFAULT_R),
            new Graphics::SmudgeBrush(0, 0, DEFAULT_R),
            new Graphics::SpeedBrush(0, 0, DEFAULT_R),
            new Graphics::SprayBrush(0, 0, DEFAULT_R),
            new Graphics::Fill(),
};
Graphics::Brush* curr_brush;
auto* curs = new Core::Cursor;

u8 curr_tool_idx = SIMPLE_IDX;

void init_brushes()
{
}
void canvas_commands(Events::Keyboard::KeyPress* kb, Graphics::Canvas* c)
{

    using namespace Events::Keyboard;
    switch(kb->get_key())
    {
        case Key::R:
            {
                c->fill(WHITE);
            }
            break;
        case Key::i:
            {
                if (tools[curr_tool_idx]->get_type() == Graphics::ToolType::Brush)
                {
                    ((Graphics::Brush*)tools[curr_tool_idx])->toggle_check_alpha();
                }
            }
            break;
        case Key::W:
            {
                if (c->wrap == Graphics::WrapMode::Clamp)
                {
                    c->wrap = Graphics::WrapMode::Around;
                }
                else 
                {
                    c->wrap = Graphics::WrapMode::Clamp;
                }
            }
            break;
        case Key::z:
            {
                switch(curr_tool_idx)
                {
                    case SMUDGE_IDX:
                        {
                        }break;
                    case FILL_IDX:
                        {
                            auto* b = (Graphics::Fill*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Red) - 1, ColourChannel::Red);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }break;
                    default:
                        {
                            auto* b = (Graphics::SimpleBrush*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Red) - 1, ColourChannel::Red);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }
                }
            }break;

        case Key::Z:
            {
                switch(curr_tool_idx)
                {
                    case SMUDGE_IDX:
                        {
                        }break;
                    case FILL_IDX:
                        {
                            auto* b = (Graphics::Fill*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Red) + 1, ColourChannel::Red);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }break;
                    default:
                        {
                            auto* b = (Graphics::SimpleBrush*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Red) + 1, ColourChannel::Red);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }
                }
            }break;
        case Key::x:
            {
                switch(curr_tool_idx)
                {
                    case SMUDGE_IDX:
                        {
                        }break;
                    case FILL_IDX:
                        {
                            auto* b = (Graphics::Fill*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Green) - 1, ColourChannel::Green);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }break;
                    default:
                        {
                            auto* b = (Graphics::SimpleBrush*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Green) - 1, ColourChannel::Green);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }
                }
            }break;

        case Key::X:
            {
                switch(curr_tool_idx)
                {
                    case SMUDGE_IDX:
                        {
                        }break;
                    case FILL_IDX:
                        {
                            auto* b = (Graphics::Fill*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Green) + 1, ColourChannel::Green);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }break;
                    default:
                        {
                            auto* b = (Graphics::SimpleBrush*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Green) + 1, ColourChannel::Green);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }
                }
            }break;
        case Key::c:
            {
                switch(curr_tool_idx)
                {
                    case SMUDGE_IDX:
                        {
                        }break;
                    case FILL_IDX:
                        {
                            auto* b = (Graphics::Fill*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Blue) - 1, ColourChannel::Blue);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }break;
                    default:
                        {
                            auto* b = (Graphics::SimpleBrush*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Blue) - 1, ColourChannel::Blue);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }
                }
            }break;

        case Key::C:
            {
                switch(curr_tool_idx)
                {
                    case SMUDGE_IDX:
                        {
                        }break;
                    case FILL_IDX:
                        {
                            auto* b = (Graphics::Fill*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Blue) + 1, ColourChannel::Blue);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }break;
                    default:
                        {
                            auto* b = (Graphics::SimpleBrush*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Blue) + 1, ColourChannel::Blue);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }
                }
            }break;
        case Key::v:
            {
                switch(curr_tool_idx)
                {
                    case SMUDGE_IDX:
                        {
                        }break;
                    case FILL_IDX:
                        {
                            auto* b = (Graphics::Fill*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Alpha) - 1, ColourChannel::Alpha);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }break;
                    default:
                        {
                            auto* b = (Graphics::SimpleBrush*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Alpha) - 1, ColourChannel::Alpha);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }
                }
            }break;

        case Key::V:
            {
                switch(curr_tool_idx)
                {
                    case SMUDGE_IDX:
                        {
                        }break;
                    case FILL_IDX:
                        {
                            auto* b = (Graphics::Fill*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Alpha) + 1, ColourChannel::Alpha);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }break;
                    default:
                        {
                            auto* b = (Graphics::SimpleBrush*)tools[curr_tool_idx];
                            auto c = b->get_colour();
                            Graphics::Utils::set_channel(&c, Graphics::Utils::get_channel(c, ColourChannel::Alpha) + 1, ColourChannel::Alpha);
                            b->set_colour(c);
                            std::cerr << "Col: "<< std::hex << b->get_colour() << '\n';
                        }
                }
            }break;
        default:{}
    }
}
void pick_tool(Events::Keyboard::KeyPress* kb)
{
    using namespace Events::Keyboard;
    switch(kb->get_key())
    {
        case Key::f:
            {
                curr_tool_idx = FILL_IDX;
            }
            break;
        case Key::One:
            {
                std::cerr << "Classic\n";
                curr_tool_idx = SIMPLE_IDX;;
            }
            break;
        case Key::Two:
            {
                std::cerr << "Linear\n";
                curr_tool_idx = LINEAR_IDX;
            }break;
        case Key::Three:
            {
                std::cerr << "Quadratic\n";
                curr_tool_idx = QUADRATIC_IDX;
            }break;
        case Key::Four:
            {
                std::cerr << "Smudge\n";
                curr_tool_idx = SMUDGE_IDX;
            }break;
        case Key::Five:
            {
                std::cerr << "Speed\n";
                curr_tool_idx = SPEED_IDX;
            }break;
        case Key::Six:
            {
                std::cerr << "Spray\n";
                curr_tool_idx = SPRAY_IDX;
            }break;
        case Key::o:
            {
                if (tools[curr_tool_idx]->get_type() == Graphics::ToolType::Brush)
                {
                    auto* b = (Graphics::Brush*)tools[curr_tool_idx];
                    b->set_r(b->get_r() - (b->get_r() > 0));
                    std::cerr << "Rad: " << b->get_r() << '\n';
                }
            }break;
        case Key::p:
            {
                if (tools[curr_tool_idx]->get_type() == Graphics::ToolType::Brush)
                {
                    auto* b = (Graphics::Brush*)tools[curr_tool_idx];
                    b->set_r(b->get_r() + 1);
                    std::cerr << "Rad: " << b->get_r() << '\n';
                }
            }break;
        default: {}
    }
}
void enter_canvas(Events::Mouse::Enter* me)
{
    std::cerr << "Enter\n";
}

void leave_canvas(Events::Mouse::Leave* me)
{
    std::cerr << "Leave\n";
}
void locate_mouse(Events::Mouse::Move* me, Graphics::Canvas* c,Graphics::Tool* b)
{
    using namespace Events::Mouse;
    curs->set_x(b->get_x());
    curs->set_y(b->get_y());
    b->set_x((i64) me->get_x());
    b->set_y((i64) me->get_y());


    if ( b->get_type() == Graphics::ToolType::Brush)
    {
        if (draw_state)
        {
            b->draw(c);
        }
        else 
        {
            ((Graphics::SpeedBrush*)tools[SPEED_IDX])->set_prev_x(me->get_x());
            ((Graphics::SpeedBrush*)tools[SPEED_IDX])->set_prev_y(me->get_y());
        }
    }
}

void draw_ink(Events::Mouse::ButtonPress* me, Graphics::Canvas* c, Graphics::Tool* b)
{
    using namespace Events::Mouse;
    switch(me->get_button())
    {
        case Button::Left:
            {
                b->draw(c);
                draw_state = 1;
            }
            break;
        default: {}
    }
}

void release_mouse(Events::Mouse::ButtonRelease* me, Graphics::Canvas* c, Graphics::Tool* b)
{
    using namespace Events::Mouse;
    switch(me->get_button())
    {
        case Button::Left:
            {
                draw_state = 0;
                c->reset_belnded();
            }
            break;
        default: {}
    }
}

int main()
{
    Core::Timer t;
    srand(time(NULL));
    Window win(
            {.width = WIDTH, .height = HEIGHT, .title = "I Love Lina"}
            );
    win.request_event_type(EventType::Pointer);
    win.request_event_type(EventType::Keyboard);
    win.init();

    Graphics::Canvas* c = new Graphics::Canvas;
    c->width = win.get_width();
    c->height = win.get_height();
    c->pixels = new u32[c->width * c->height];
    c->blended = new u32[c->width * c->height];
    c->fill(WHITE);
    c->reset_belnded();



    std::function<void(Events::Keyboard::KeyPress*)> new_func = pick_tool;
    std::function<void(Events::Keyboard::KeyPress*, Graphics::Canvas*)> canvas_key = canvas_commands;
    std::function<void(Events::Mouse::ButtonPress*, 
            Graphics::Canvas* c, Graphics::Tool* b)> mouse_press = draw_ink;
    std::function<void(Events::Mouse::Move*, Graphics::Canvas* c, Graphics::Tool* b)> 
        mouse_move 
        = locate_mouse;

    std::function<void(Events::Mouse::ButtonRelease*, Graphics::Canvas* c, Graphics::Tool* b)> 
        mrel 
        = release_mouse;

    std::function<void(Events::Mouse::Leave*)> 
        mleave 
        = leave_canvas;

    std::function<void(Events::Mouse::Enter*)> 
        menter 
        = enter_canvas;

    while (win.update()){
        t.begin();
        win.request_subscribe<Events::Keyboard::KeyPress>(new_func);
        win.request_subscribe<Events::Keyboard::KeyPress>(canvas_key, c);
        win.request_subscribe<Events::Mouse::ButtonPress>(mouse_press, c, tools[curr_tool_idx]);
        win.request_subscribe<Events::Mouse::ButtonRelease>(mrel, c, tools[curr_tool_idx]);
        win.request_subscribe<Events::Mouse::Move>(mouse_move, c, tools[curr_tool_idx]);
        win.request_subscribe<Events::Mouse::Enter>(menter);
        win.request_subscribe<Events::Mouse::Leave>(mleave);
        win.handle_events();
        win.fill(c->pixels, 0, 0, c->width, c->height);
        win.outline_cursor(curs);
        t.end();
        //t.wait(1000.0f/60 - t.get_time());
    }
    win.wclose();

}
