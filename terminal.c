#include "terminal.h"

#include <stdint.h>

void draw_text_area(WINDOW* text_window, GapBuffer* gap_buffer, int base_pos) {
    werase(text_window);

    int max_lines = LINES - 2;
    int max_cols = COLS;
    int line = 0, col = 0;

    for (int i = base_pos; i < gap_buffer->size && line < max_lines; i++) {
        if (i >= gap_buffer->gap_start && i < gap_buffer->gap_end) {
            continue;
        }
        if (gap_buffer->char_buffer[i] == '\0') { continue; }
        waddch(text_window, gap_buffer->char_buffer[i]);
        col++;
        if (col >= max_cols) {
            col = 0;
            line++;
        }
    }
    wrefresh(text_window);
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

void calculate_screen_1dim_pos(GapBuffer* gap_buffer, int base_1dim_pos, int crnt_screen_cursor_rel_1dim_pos, int* calculated_screen_cursor_1dim_pos) {
    int relative_pos = base_1dim_pos + crnt_screen_cursor_rel_1dim_pos;
    int current_line_chars = 0;
    int total_cols = 0;

    for (int i = base_1dim_pos; i < relative_pos; i++) {
        char current_char = gap_buffer->char_buffer[i];

        if (current_line_chars >= COLS) {
            total_cols += COLS;
            current_line_chars = 0;
        }

        if (current_char == '\n') {
            total_cols += COLS;
            current_line_chars = 0;
        } else {
            current_line_chars++;
        }
    }

    if (current_line_chars >= 0) {
        if (current_line_chars >= COLS) {
            total_cols += COLS;
        } else {
            total_cols += current_line_chars;
        }
    }

    *calculated_screen_cursor_1dim_pos = total_cols;
}

void draw_find_default_status_bar(WINDOW* status_window) {
    char* temp = "Type word for search, if done press enter to start search";
    char* status = (char*)malloc(COLS * sizeof(char));
    snprintf(status, COLS,"%s %*s",temp,COLS-strlen(temp),"");
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    wattron(status_window, COLOR_PAIR(1));
    mvwprintw(status_window, 0, 0, status);
    wattroff(status_window, COLOR_PAIR(1));
    free(status);
}

void draw_find_info_status_bar(WINDOW* status_window) {
    char* temp = "Press <- & -> to move";
    char* status = (char*)malloc(COLS * sizeof(char));
    snprintf(status, COLS,"%s %*s",temp,COLS-strlen(temp),"");
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    wattron(status_window, COLOR_PAIR(1));
    mvwprintw(status_window, 0, 0, status);
    wattroff(status_window, COLOR_PAIR(1));
    free(status);
}


// WILL BE DEPRECATED.
void draw_find_result_message(WINDOW* message_window, GapBuffer* word_buffer, int scr_x, int scr_y) {
    char* message = (char*)malloc(COLS * sizeof(char));
    size_t word_length = 0;
    for(int i = 0; i < word_buffer->size; i++) {
        if(word_buffer->char_buffer[i] != '\0') {
            word_length++;
        }
    }
    char* word = (char*)calloc(word_length, sizeof(char));
    strncpy(word, word_buffer->char_buffer, word_length);
    snprintf(message, COLS,"Search:%s %*s %d / %d",word, COLS-strlen(word)-25, "", scr_x, scr_y);
    mvwprintw(message_window, 0, 0, message);
    free(message);
    free(word);
}

// void find_word_position(GapBuffer* gap_buffer, GapBuffer* word_buffer, int* gap_buffer_cursor_position) {
//
// }