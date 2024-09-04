#include <stdio.h>

typedef struct {
    int row;
    int col;
}Cursor;

#ifdef _WIN32
#include <windows.h>
void get_console_size(int* rows, int* cols);
void enable_raw_mode(HANDLE hConsole, DWORD* crntMode);
void process_special_key_events(WORD virtualKeyCode, Cursor* cursor);
char process_key_events(KEY_EVENT_RECORD keyEvent, Cursor* cursor);
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
void get_console_size(int* rows, int* cols);
void enable_raw_mode(struct termios* orig_termios);
#endif

Cursor* create_new_cursor();
void move_cursor(int row, int col);
void set_background_color(int color);
void set_text_color(int color);
void reset_colors();
void clear_screen();