#ifndef terminal_h
#define terminal_h

#include <stdio.h>
#include "gap_buffer.h"

typedef struct {
    int row;
    int col;
}Cursor;

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
void get_console_size(int* height, int* width);
void enable_raw_mode(HANDLE hConsole, DWORD* crntMode);
void handle_key(Cursor* cursor, GapBuffer* gap_buffer, int terminal_height, int terminal_width);
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
void get_console_size(int* height, int* width);
void enable_raw_mode(struct termios* orig_termios);
#endif

Cursor* create_new_cursor();
void move_cursor(int row, int col);
int flatten_cursor_position(Cursor* cursor, int terminal_width);
void set_background_color(int color);
void set_text_color(int color);
void reset_colors();
void clear_screen();

#endif