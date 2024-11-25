#include "terminal.h"

void draw_text_area(WINDOW* text_window, GapBuffer* gap_buffer) {
    if (gap_buffer->gap_start > 0) {
        waddnstr(text_window, gap_buffer->char_buffer, gap_buffer->gap_start);
    }
    if (gap_buffer->gap_end < gap_buffer->size - 1) {
        waddnstr(text_window,
            gap_buffer->char_buffer + gap_buffer->gap_end + 1,
            gap_buffer->size - gap_buffer->gap_end - 1);
    }
}

void draw_status_bar(int width, char* file_name, char* file_extension, int current_line, int total_lines, WINDOW* status_window) {
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    wattron(status_window, COLOR_PAIR(1));

    int temp_crnt_line = current_line;
    int temp_total_line = total_lines;
    int crnt_line_len = 0;
    int total_line_len = 0;
	char* no_extension = "no ft";
	char* no_name = "No name";

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


    char* status = (char*)malloc(width + 1);
    if (strlen(file_extension) > 0)
    {
        int blanks_count = width - (int)strlen(file_name) - (int)strlen(file_extension) - 17 - crnt_line_len * 2 - total_line_len;
        snprintf(status, width + 1, "[%s] - %d lines %*s %s | %d/%d",
            file_name, current_line,
            blanks_count, "",
            file_extension, current_line, total_lines);
    }
    else {
		if (strlen(file_name) > 0) {
            int blanks_count = width - (int)strlen(file_name) - 5 - 17 - crnt_line_len * 2 - total_line_len;
            snprintf(status, width + 1, "[%s] - %d lines %*s %s | %d/%d",
                file_name, current_line,
                blanks_count, "",
                no_extension, current_line, total_lines);
        }
        else {
            int blanks_count = width - (int)strlen(no_name) - 5 - 17 - crnt_line_len * 2 - total_line_len;
            snprintf(status, width + 1, "[%s] - %d lines %*s %s | %d/%d",
                no_name, current_line,
                blanks_count, "",
                no_extension, current_line, total_lines);
        }
    }

    mvwprintw(status_window, 0, 0, "%s", status);
    wattroff(status_window, COLOR_PAIR(1));
}

void draw_default_message_bar(WINDOW* message_window) {
	mvwprintw(message_window, 0, 0, "HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");
}

void draw_quit_message_bar(WINDOW* message_window) {
    mvwprintw(message_window, 0, 0, "Really Exit vite? | press 'Ctrl-Q' once more to exit | press any key to cancel");
}

void draw_save_complete_message_bar(WINDOW* message_window) {
    mvwprintw(message_window, 0, 0, "File Saved!");
}

void refresh_screen(WINDOW* text_window, WINDOW* status_window, WINDOW* message_window) {
	wrefresh(text_window);
	wrefresh(status_window);
	wrefresh(message_window);
}

void calc_opening_position(const GapBuffer* gap_buffer, int* scr_csr_x, int* scr_csr_y, int gap_start) {
    for (int i = 0; i < gap_start; i++) {
        if (gap_buffer->char_buffer[i] == '\n') {
            (*scr_csr_y)++;
            *scr_csr_x = 0;
        } else {
            (*scr_csr_x)++;
            if (*scr_csr_x >= COLS) {
                *scr_csr_x = 0;
                (*scr_csr_y)++;
            }
        }
    }

    if (*scr_csr_y > LINES-2) {
        *scr_csr_y = LINES-2;
    }
}

void calc_cursor_position(GapBuffer* gap_buffer, int* gap_buffer_cursor_position, int* scr_csr_x, int* scr_csr_y) {
    int y_pos = 0;
    int chars_in_current_line = 0;
    int current_line_start = 0;

    for (int i = 0; i < *gap_buffer_cursor_position; i++) {
        if (gap_buffer->char_buffer[i] == '\n') {
            y_pos += (chars_in_current_line / COLS) + 1;
            chars_in_current_line = 0;
            current_line_start = i + 1;
        } else {
            chars_in_current_line++;
        }
    }

    int current_line_length = *gap_buffer_cursor_position - current_line_start;
    y_pos += current_line_length / COLS;
    int x_pos = current_line_length % COLS;
    if (*gap_buffer_cursor_position > 0 &&
        gap_buffer->char_buffer[*gap_buffer_cursor_position - 1] == '\n') {
        x_pos = 0;
        }

    *scr_csr_x = x_pos;
    *scr_csr_y = y_pos;
}
