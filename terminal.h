#ifndef TERMINAL_H
#define TERMINAL_H



#define WIN64_KEY_HOME 449
#define WIN64_KEY_END 455
#define WIN64_KEY_BACKSPACE 8
#define KEY_CTRL_Q 17
#define KEY_CTRL_S 19
#define KEY_CTRL_F 6
#define WIN64_KEY_PAGE_UP 451
#define WIN64_KEY_PAGE_DOWN 457
#define MAC_BACKSPACE 127
#define MAC_CTRL_F 6

#include <stdio.h>
#include "gap_buffer.h"
#include <curses.h>

void draw_text_area(WINDOW* text_window, GapBuffer* gap_buffer, int base_pos);
void draw_status_bar(int width, char* file_name, char* file_extension, int current_line, int total_lines, WINDOW* status_window);
void draw_default_message_bar(WINDOW* message_window);
void refresh_screen(WINDOW* text_window, WINDOW* status_window, WINDOW* message_window);
void draw_quit_message_bar(WINDOW* message_window);
void draw_save_complete_message_bar(WINDOW* message_window);
int calc_render_start_index(GapBuffer* gap_buffer, int current_cursor_row);
void calculate_screen_1dim_pos(GapBuffer* gap_buffer, int base_1dim_pos, int crnt_screen_cursor_rel_1dim_pos, int* calculated_screen_cursor_1dim_pos);
void find_word_position(GapBuffer* gap_buffer, GapBuffer* word_buffer, int* gap_buffer_cursor_position, WINDOW* message_window, WINDOW* status_window);
#endif // TERMINAL_H
