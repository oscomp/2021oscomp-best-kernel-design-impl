
#ifndef INCLUDE_CONSOLE_H_
#define INCLUDE_CONSOLE_H_

extern struct ring_buffer stdin_buf;
extern struct ring_buffer stdout_buf;
extern struct ring_buffer stderr_buf;

/* configuring screen properties */
void init_screen(void);

/* reflush screen buffer */
void screen_reflush(void);

/* just go to assert */
void screen_assert(void);

#endif