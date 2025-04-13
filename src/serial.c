#include "../include/io.h"
#include "../include/serial.h"

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

#define SERIAL_LINE_ENABLE_DLAB         0x80

void serial_configure_baud_rate(unsigned short com, unsigned short divisor) {
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

void serial_configure_line(unsigned short com) {
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

void serial_configure_buffers(unsigned short com) {
    outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

void serial_configure_modem(unsigned short com) {
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x0B);
}

void serial_write(char *buf, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        while (!(inb(SERIAL_LINE_STATUS_PORT(SERIAL_COM1_BASE)) & 0x20));
        outb(SERIAL_COM1_BASE, buf[i]);
    }
}

char serial_read() {
    while (!(inb(SERIAL_LINE_STATUS_PORT(SERIAL_COM1_BASE)) & 1));
    return inb(SERIAL_COM1_BASE);
}
