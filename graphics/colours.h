#ifndef COLOURS_H
#define COLOURS_H
#define BLACK (0xff000000)
#define WHITE (0xffffffff)
#define RED (0xffff0000)
#define GREEN (0xff00ff00)
#define BLUE (0xff0000ff)
#define TRANSRED (0x44ff0000)
#define TRANSBLACK (0x44000000)

enum class ColourChannel
{
    Blue,
    Green,
    Red,
    Alpha
};
#endif
