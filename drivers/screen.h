
#ifndef INCLUDE_CONSOLE_H_
#define INCLUDE_CONSOLE_H_

extern int screen_cursor_x;
extern int screen_cursor_y;

void vt100_move_cursor(int, int);

/* configuring screen properties */
void init_screen(void);

/* clear screen */
void screen_clear(void);

/* reflush screen buffer */
void screen_reflush(void);

/* screen write string */
void screen_write(char *buff);

/* move cursor int (x,y) */
void screen_move_cursor(int x, int y);

#endif