g++  -std=c++23\
    -Werror -Wall -pedantic -g \
    internal.cc xdg-shell-proto.cc \
    events/wayland_pointer_reader.cc events/wayland_keyboard_reader.cc events/event_handler.cc\
    window.cc\
    graphics/utils.cc graphics/canvas.cc graphics/shapes.cc graphics/brush.cc graphics/fill.cc\
    core/cursor.cc core/timer.cc\
    main.cc -o main -lrt -lwayland-client -lxkbcommon;
