#include "terminal.h"

void draw_text_area(WINDOW* text_window, GapBuffer* gap_buffer) {
	waddnstr(text_window, gap_buffer->char_buffer, gap_buffer->size);
	waddnstr(text_window, gap_buffer->char_buffer + gap_buffer->gap_end + 1, gap_buffer->size - gap_buffer->gap_end - 1);
}



void draw_status_bar(int width, char* file_name, char* file_extension, int current_line, int total_lines, WINDOW* status_window) {
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    wattron(status_window, COLOR_PAIR(1));

    int temp_crnt_line = current_line;
    int temp_total_line = total_lines;
    int crnt_line_len = 0;
    int total_line_len = 0;

    if (temp_crnt_line == 0) {
        crnt_line_len = 1;
    }
    else {
        while (temp_crnt_line != 0) {
            temp_crnt_line /= 10;
            crnt_line_len++;
        }
    }

    if (temp_total_line == 0) {
        total_line_len = 1;
    }
    else {
        while (temp_total_line != 0) {
            temp_total_line /= 10;
            total_line_len++;
        }
    }
    
    int blanks_count = width - (int)strlen(file_name) - (int)strlen(file_extension) - 17 - crnt_line_len*2 - total_line_len;
    char* status = (char*)malloc(width + 1);
    
    snprintf(status, width + 1, "[%s] - %d lines %*s %s | %d/%d",
        file_name, current_line,
        blanks_count, "",
        file_extension, current_line, total_lines);
    
    mvwprintw(status_window, 0, 0, "%s", status);
    wattroff(status_window, COLOR_PAIR(1));
}

void draw_default_message_bar(WINDOW* message_window) {
	mvwprintw(message_window, 0, 0, "HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");
}

void draw_quit_message_bar(WINDOW* message_window) {
    mvwprintw(message_window, 0, 0, "Really Exit vite? | press 'Ctrl-Q' once more to exit | press any key to cancel");
}

void refresh_screen(WINDOW* text_window, WINDOW* status_window, WINDOW* message_window) {
	wrefresh(text_window);
	wrefresh(status_window);
	wrefresh(message_window);
}

void move_cursor(WINDOW* text_window, int* scr_csr_x, int* scr_csr_y) {

}