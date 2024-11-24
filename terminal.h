#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdio.h>
#include "gap_buffer.h"
#include <curses.h>

void draw_text_area(WINDOW* text_window, GapBuffer* gap_buffer);
void draw_status_bar(int width, char* file_name, char* file_extension, int current_line, int total_lines, WINDOW* status_window);
void draw_default_message_bar(WINDOW* message_window);
void refresh_screen(WINDOW* text_window, WINDOW* status_window, WINDOW* message_window);
void draw_quit_message_bar(WINDOW* message_window);
void move_cursor(WINDOW* text_window, int* scr_csr_x, int* scr_csr_y);
void calc_screen_cursor_position(int* scr_cursor_x, int* scr_cursor_y);
void calc_opening_position(const GapBuffer* gap_buffer, int* scr_csr_x, int* scr_csr_y, int gap_start);
void draw_save_complete_message_bar(WINDOW* message_window);

#endif // TERMINAL_H
