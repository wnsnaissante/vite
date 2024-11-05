#ifndef terminal_h
#define terminal_h

#include <stdio.h>
#include "gap_buffer.h"

typedef struct {
    int buffer_pos;   
    int virtual_row;  
    int virtual_col;  
} Cursor;

Cursor* create_new_cursor();
int flatten_cursor_pos(Cursor* cursor, int terminal_width);
void buffer_idx_to_cursor_pos(Cursor* cursor, int terminal_width);

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
void get_console_size(int* height, int* width);
void enable_raw_mode(HANDLE hConsole, DWORD* crntMode);
void handle_key(Cursor* cursor, GapBuffer* gap_buffer, int terminal_height, int terminal_width, int* scroll_offset);
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
void get_console_size(int* height, int* width);
void enable_raw_mode(struct termios* orig_termios);
#endif


#endif