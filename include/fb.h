#ifndef INCLUDE_FB_H
#define INCLUDE_FB_H

extern unsigned short cursor_position;
extern const unsigned int FB_WIDTH;

void write(const char *buf, unsigned int len);
void clear_screen();
void move_cursor(unsigned int row, unsigned int col);
void newline();

#endif /* INCLUDE_FB_H */
