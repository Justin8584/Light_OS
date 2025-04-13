#include "../include/fb.h"
#include "../include/io.h"

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

const unsigned int FB_WIDTH = 80;
const unsigned int FB_HEIGHT = 25;
const unsigned int FB_SIZE = FB_WIDTH * FB_HEIGHT * 2;
const unsigned int FB_ADDRESS = 0xB8000;

unsigned short cursor_position = 0;

void write(const char *buf, unsigned int len)
{
    volatile char *fb = (volatile char *)FB_ADDRESS;
    for (unsigned int i = 0; i < len; i++)
    {
        if (buf[i] == '\n')
        {
            newline();
            continue;
        }
        if (buf[i] == '\t')
        {
            for (int j = 0; j < 4; ++j)
            {
                if (cursor_position >= FB_WIDTH * FB_HEIGHT)
                {
                    clear_screen();
                    cursor_position = 0;
                }
                fb[cursor_position * 2] = ' ';
                fb[cursor_position * 2 + 1] = 0x07;
                cursor_position++;
                move_cursor(cursor_position / FB_WIDTH, cursor_position % FB_WIDTH);
            }
            continue;
        }

        if (cursor_position >= FB_WIDTH * FB_HEIGHT)
        {
            clear_screen();
            cursor_position = 0;
        }

        fb[cursor_position * 2] = buf[i];
        fb[cursor_position * 2 + 1] = 0x07;
        cursor_position++;

        move_cursor(cursor_position / FB_WIDTH, cursor_position % FB_WIDTH);
    }
}

void clear_screen()
{
    volatile char *fb = (volatile char *)FB_ADDRESS;
    for (unsigned int i = 0; i < FB_SIZE; i += 2)
    {
        fb[i] = ' ';
        fb[i + 1] = 0x07;
    }
    cursor_position = 0;
    move_cursor(0, 0);
}

void move_cursor(unsigned int row, unsigned int col)
{
    cursor_position = row * FB_WIDTH + col;
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((cursor_position >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, cursor_position & 0x00FF);
}

void newline()
{
    cursor_position = ((cursor_position / FB_WIDTH) + 1) * FB_WIDTH;
    if (cursor_position >= FB_WIDTH * FB_HEIGHT)
    {
        clear_screen();
        cursor_position = 0;
    }
    move_cursor(cursor_position / FB_WIDTH, cursor_position % FB_WIDTH);
}
