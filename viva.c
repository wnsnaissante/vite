// 2024, Sophomore 2nd semester Advanced Data Structure Project

/*
*   Initiation date : 2024/09/02
*   Copyright (C) 2024 Junyoung Kim
*/

/*
    Tested On :
                1. Windows 10(11), gcc (MinGW.org GCC-6.3.0-1) 6.3.0

                2. Ubuntu-22.04LTS, gcc 11.3.0-1ubuntu1~22.04.1 11.3.0
                                  Ubuntu clang version 14.0.0-1ubuntu1.1
                                  Target: x86_64-pc-linux-gnu
                                  Thread model: posix

                3. macOS Sonoma 14.5, Apple clang version 15.0.0 (clang-1500.3.9.4)
                                    Target: x86_64-apple-darwin23.5.0
                                    Thread model: posix
*/

#define _CRT_SECURE_NO_WARNINGS

#define WIN64_KEY_HOME 449
#define WIN64_KEY_END 455
#define WIN64_KEY_BACKSPACE 8
#define WIN64_KEY_CTRL_Q 17
#define WIN64_KEY_CTRL_S 19
#define WIN64_KEY_CTRL_F 6
#define WIN64_KEY_PAGE_UP 451
#define WIN64_KEY_PAGE_DOWN 457

#include "terminal.h"
#include "gap_buffer.h"

static GapBuffer* filename_buffer;

static char file_name[100];
static char file_extension[10];


static _Bool is_saved = 0;
static int scr_csr_x = 0;
static int scr_csr_y = 0;
static int gap_buffer_cursor_1d_position = 0;
static int total_lines = 0;

void handle_key_(WINDOW* text_window, WINDOW* status_window, WINDOW* message_window, GapBuffer* gap_buffer) {
    draw_default_message_bar(message_window);
    draw_status_bar(COLS, file_name, file_extension, scr_csr_y, get_total_lines(gap_buffer), status_window);
    refresh();
    int ch = getch();
    switch (ch) {
    case WIN64_KEY_CTRL_Q:
        if (strlen(gap_buffer->char_buffer) == 0 || is_saved == 1) {
            exit(0);
            break;
        }
        else {
            draw_quit_message_bar(message_window);
            refresh_screen(text_window, status_window, message_window);
            ch = getch();
            if (ch == 17) {
                exit(0);
            }
            else {
                werase(message_window);
                draw_default_message_bar(message_window);
                wrefresh(message_window);
                break;
            }
        }
        break;
    case WIN64_KEY_CTRL_S: // Ctrl-S
        if (strlen(file_name) == 0 && strlen(file_extension) == 0) {
            werase(message_window);
            waddstr(message_window, "Type filename.extension");
            wrefresh(message_window);
            char input_char;
            int position = 0;
            do {
                input_char = getch();
                if (input_char != '\n') {
                    if (input_char == KEY_BACKSPACE || input_char == WIN64_KEY_BACKSPACE) {
                        delete_char(filename_buffer, position);
                        position--;
                    }
                    insert_char(filename_buffer, input_char, position++);
                    werase(message_window);
                    waddstr(message_window, filename_buffer->char_buffer);
                    wrefresh(message_window);
                }
            } while (input_char != '\n');
            if (strpbrk(filename_buffer->char_buffer, ".") != NULL) {
                strcpy(file_extension, strpbrk(filename_buffer->char_buffer, ".")); // get file extension
                strcpy(file_name, strtok(filename_buffer->char_buffer, ".")); // get file name
            }
            else {
                strcpy(file_name, filename_buffer->char_buffer);
                strcpy(file_extension, "\0");
            }
        }
        is_saved = 1;
        save_to_file(gap_buffer, file_name, file_extension);
        werase(message_window);
        draw_save_complete_message_bar(message_window);
        wrefresh(message_window);
        break;
    case WIN64_KEY_CTRL_F: // Ctrl-F
        break;
    case KEY_UP:
        break;
    case KEY_DOWN:
        break;
    case KEY_LEFT:
        break;
    case KEY_RIGHT:
        break;
    case KEY_BACKSPACE:
    case WIN64_KEY_BACKSPACE:
        if (gap_buffer->char_buffer[gap_buffer_cursor_1d_position - 1] == '\n') {
            total_lines--;
        }
        is_saved = 0;
        if (scr_csr_y > 0) {
            if (scr_csr_x >= 1) {
                delete_char(gap_buffer, gap_buffer_cursor_1d_position);
                gap_buffer_cursor_1d_position--;
                scr_csr_x--;
                move(scr_csr_y, scr_csr_x);
                break;
            }
            else {
                delete_char(gap_buffer, gap_buffer_cursor_1d_position);
                calc_last_line_change_position(gap_buffer, gap_buffer_cursor_1d_position, &scr_csr_x, &scr_csr_y);
                if (scr_csr_y >= LINES - 2) {
                    scr_csr_y = LINES - 2;
                    wscrl(text_window, -1);
                    refresh();
                }
                move(scr_csr_y, scr_csr_x);
                gap_buffer_cursor_1d_position--;

                break;
            }
        }
        else {
            if (scr_csr_x >= 1) {
                delete_char(gap_buffer, gap_buffer_cursor_1d_position);
                gap_buffer_cursor_1d_position--;
                scr_csr_x--;
                move(scr_csr_y, scr_csr_x);
                break;
            }
        }
        break;
    case KEY_HOME:
    case WIN64_KEY_HOME: // 노트북 홈키

        break;
    case WIN64_KEY_END:
    case KEY_END:
        break;
    case KEY_NPAGE:
    case WIN64_KEY_PAGE_DOWN:
        wscrl(text_window, LINES - 2);
        wrefresh(text_window);
        wrefresh(text_window);
        wmove(text_window, LINES - 2, 0);
        break;
    case KEY_PPAGE:
    case WIN64_KEY_PAGE_UP:
        wscrl(text_window, -(LINES - 2));
        wrefresh(text_window);
        wmove(text_window, LINES - 2, 0);
        break;
    default:
        is_saved = 0;
        insert_char(gap_buffer, ch, gap_buffer_cursor_1d_position);
        gap_buffer_cursor_1d_position++;
        if (ch == '\n') {
            total_lines++;
        }
        if (scr_csr_x == COLS) {
            scr_csr_x = 0;
            wprintw(text_window, "\n");
            scr_csr_y++;
            total_lines++;
        }

        if (ch == 10) {
            scr_csr_x = 0;
            if (scr_csr_y <= LINES - 2)
            {
                scr_csr_y++;
            }
            else {
                scroll(text_window);
            }

            move(scr_csr_y, scr_csr_x);
        }
        else {
            scr_csr_x++;
            move(scr_csr_y, scr_csr_x);
        }
        if (scr_csr_y == LINES - 2) {
            scr_csr_y--;
            move(scr_csr_y, scr_csr_x);
        }
        move(scr_csr_y, scr_csr_x);
        break;
    }
}

int main(int argc, char* argv[]) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    filename_buffer = create_gap_buffer(FILENAME_MAX);

    GapBuffer* gap_buffer = create_gap_buffer(1024);
    if (argc > 1)   // get file name and its extension from parameter
    {
        char* original_file_name = argv[1];
        if (strpbrk(original_file_name, ".") != NULL) {
            strcpy(file_extension, strpbrk(original_file_name, ".")); // get file extension
            strcpy(file_name, strtok(original_file_name, ".")); // get file name
        }
        else {
			strcpy(file_name, original_file_name);
			strcpy(file_extension, "\0");
        }
        gap_buffer_cursor_1d_position = open_file(gap_buffer, file_name, file_extension);
        calc_opening_position(gap_buffer, &scr_csr_x, &scr_csr_y, gap_buffer_cursor_1d_position);
        total_lines = get_total_lines(gap_buffer);
        free(filename_buffer);
    }
    else {
        strcpy(file_name, "\0");
		strcpy(file_extension, "\0");
    }


    // Text Area
    WINDOW* text_window = newwin(LINES - 2, COLS, 0, 0);

    // Status Area
    WINDOW* status_window = newwin(1, COLS, LINES - 2, 0);

    // Message Area
    WINDOW* message_window = newwin(1, COLS, LINES - 2 + 1, 0);
    scrollok(text_window, TRUE);

    while (1) {
        if (scr_csr_y > LINES - 2) {
            while (scr_csr_y < LINES - 2) {
                scr_csr_y--;
                wmove(text_window, LINES - 2, scr_csr_x);
                wrefresh(text_window);
            }
        }
        handle_key_(text_window, status_window, message_window, gap_buffer);
        if (scr_csr_y == LINES - 2) {
            scroll(text_window);
            scr_csr_y--;
            move(scr_csr_y, scr_csr_x);
            wrefresh(text_window);
        }

        werase(text_window);
        draw_status_bar(COLS, file_name, file_extension, scr_csr_y, total_lines, status_window);
        //draw_status_bar(COLS, file_name, file_extension, scr_csr_x, scr_csr_y, status_window);
        draw_text_area(text_window, gap_buffer);
        refresh_screen(text_window, status_window, message_window);
    }
    return 0;
}
