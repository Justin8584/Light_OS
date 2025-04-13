#include "../include/keyboard.h"
#include "../include/io.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_STATUS_MASK_OUT_BUF 1

char scancode_to_ascii(unsigned char scancode) {
    static char ascii_table[128] = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
        '9', '0', '-', '=', '\b', /* Backspace */
        '\t', /* Tab */
        'q', 'w', 'e', 'r', /* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
        0, /* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
        '\'', '`', 0, /* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n', /* 49 */
        'm', ',', '.', '/', 0, /* Right shift */
        '*',
        0, /* Alt */
        ' ', /* Space bar */
        0, /* Caps lock */
        0, 0, 0, 0, 0, 0, 0, 0, 0, /* F1-F9 */
        0, /* F10 */
        0, /* Num lock */
        0, /* Scroll Lock */
        0, /* Home key */
        0, /* Up Arrow */
        0, /* Page Up */
        '-',
        0, /* Left Arrow */
        0,
        0, /* Right Arrow */
        '+',
        0, /* End key */
        0, /* Down Arrow */
        0, /* Page Down */
        0, /* Insert Key */
        0, /* Delete Key */
        0, 0, 0,
        0, /* F11 Key */
        0, /* F12 Key */
        0 /* All other keys are undefined */
    };

    if (scancode > 128) {
        return 0;
    }
    return ascii_table[scancode];
}

char keyboard_read_char() {
    while (!(inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_MASK_OUT_BUF)) {
        // Wait for the keyboard buffer to be full
    }
    unsigned char scancode = inb(KEYBOARD_DATA_PORT);
    return scancode_to_ascii(scancode);
}

