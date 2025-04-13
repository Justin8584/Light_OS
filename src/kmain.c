
#include "../include/fs.h"
#include "../include/fb.h"
#include "../include/serial.h"
#include "../include/shell.h"
#include "../include/keyboard.h"

void kmain(void)
{
    clear_screen();

    serial_configure_baud_rate(SERIAL_COM1_BASE, 1);
    serial_configure_line(SERIAL_COM1_BASE);
    serial_configure_buffers(SERIAL_COM1_BASE);
    serial_configure_modem(SERIAL_COM1_BASE);

    char *message = "Welcome to Light_OS\n";
    write(message, 18);

    fs_init();
    write("File system initialized.\n", 25);

    shell();
}
