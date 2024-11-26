#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdio.h>
#include "gap_buffer.h"
#include <curses.h>

void draw_text_area(WINDOW* text_window, GapBuffer* gap_buffer, int base_pos);
void draw_status_bar(int width, char* file_name, char* file_extension, int current_line, int total_lines, WINDOW* status_window);
void draw_default_message_bar(WINDOW* message_window);
void refresh_screen(WINDOW* text_window, WINDOW* status_window, WINDOW* message_window);
void draw_quit_message_bar(WINDOW* message_window);
void move_cursor(WINDOW* text_window, int* scr_csr_x, int* scr_csr_y);
void calc_opening_position(const GapBuffer* gap_buffer, int* scr_csr_x, int* scr_csr_y, int gap_start);
void draw_save_complete_message_bar(WINDOW* message_window);
void calc_cursor_position(GapBuffer* gap_buffer, int* gap_buffer_cursor_position, int* scr_csr_x, int* scr_csr_y);
void calibration_cursor_position(GapBuffer* gap_buffer, int* base_scr_csr_x, int* base_scr_csr_y, int* relative_x, int* relative_y, int* scroll_offset);
int calc_render_start_index(GapBuffer* gap_buffer, int current_cursor_row);
void calculate_screen_1dim_pos(GapBuffer* gap_buffer, int base_1dim_pos, int crnt_screen_cursor_rel_1dim_pos, int* calculated_screen_cursor_1dim_pos);

#endif // TERMINAL_H
