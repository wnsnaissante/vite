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
#define KEY_CTRL_Q 17
#define KEY_CTRL_S 19
#define KEY_CTRL_F 6
#define WIN64_KEY_PAGE_UP 451
#define WIN64_KEY_PAGE_DOWN 457
#define MAC_BACKSPACE 127
#define MAC_CTRL_F 6

#include "terminal.h"
#include "gap_buffer.h"



static GapBuffer* filename_buffer;

static char file_name[100];
static char file_extension[10];


static _Bool is_saved = 0;
static _Bool is_recently_opened = 1;
static int base_1dim_pos = 0;
static int screen_1dim_rel_pos = 0;
static int gap_buffer_cursor_1dim_position = 0;
static int calculated_screen_1dim_pos = 0;
static int scr_x_pos_2dim = 0;
static int scr_y_pos_2dim = 0;

static int total_lines = 0;


void handle_key_(WINDOW* text_window, WINDOW* status_window, WINDOW* message_window, GapBuffer* gap_buffer) {
    draw_default_message_bar(message_window);
    draw_status_bar(COLS, file_name, file_extension, scr_y_pos_2dim, get_total_lines(gap_buffer), status_window);
    refresh();
    int ch = getch();
    switch (ch) {
    case KEY_CTRL_Q:
        if (strlen(gap_buffer->char_buffer) == 0 || is_saved == 1 || is_recently_opened == 1) {
            exit(0);
            break;
        }
        else {
            draw_quit_message_bar(message_window);
            wrefresh(message_window);
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
    case KEY_CTRL_S:
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
        if (is_recently_opened != 0) {is_recently_opened = 0;}
        save_to_file(gap_buffer, file_name, file_extension);
        werase(message_window);
        draw_save_complete_message_bar(message_window);
        wrefresh(message_window);
        break;
    case KEY_CTRL_F:
        break;
    case KEY_UP:
        break;
    case KEY_DOWN:
        break;
    case KEY_LEFT:
        gap_buffer_cursor_1dim_position--;
        screen_1dim_rel_pos--;
        calculate_screen_1dim_pos(gap_buffer, base_1dim_pos, screen_1dim_rel_pos, &calculated_screen_1dim_pos);
        break;
    case KEY_RIGHT:
        gap_buffer_cursor_1dim_position++;
        screen_1dim_rel_pos++;
        calculate_screen_1dim_pos(gap_buffer, base_1dim_pos, screen_1dim_rel_pos, &calculated_screen_1dim_pos);
        break;
    case KEY_HOME:
    case WIN64_KEY_HOME:
        while (gap_buffer_cursor_1dim_position > 0 &&
               gap_buffer->char_buffer[gap_buffer_cursor_1dim_position - 1] != '\n') {
            gap_buffer_cursor_1dim_position--;
            screen_1dim_rel_pos--;
        }
        break;

    case WIN64_KEY_END:
        case KEY_END:
        while (gap_buffer->char_buffer[gap_buffer_cursor_1dim_position] != '\n' &&
               gap_buffer_cursor_1dim_position < gap_buffer->gap_start + gap_buffer->size) {
            gap_buffer_cursor_1dim_position++;
            screen_1dim_rel_pos++;
               }

        if (gap_buffer->char_buffer[gap_buffer_cursor_1dim_position] == '\n') {
            gap_buffer_cursor_1dim_position++;
            screen_1dim_rel_pos++;
        } else if (gap_buffer->char_buffer[gap_buffer_cursor_1dim_position] == '\0') {
            gap_buffer_cursor_1dim_position++;
            screen_1dim_rel_pos++;
        }

        break;

    case KEY_NPAGE:
    case WIN64_KEY_PAGE_DOWN:
        break;
    case KEY_PPAGE:
    case WIN64_KEY_PAGE_UP:

        break;
    case KEY_BACKSPACE:
    case WIN64_KEY_BACKSPACE:
        if (gap_buffer_cursor_1dim_position > 0) {
            delete_char(gap_buffer, gap_buffer_cursor_1dim_position);
            gap_buffer_cursor_1dim_position--;
            screen_1dim_rel_pos--;
            if (is_recently_opened != 0) {is_recently_opened = 0;}
            break;
        } else {
            gap_buffer_cursor_1dim_position = 0;
            screen_1dim_rel_pos = 0;
            if (is_recently_opened != 0) {is_recently_opened = 0;}
            break;
        }

    default:
        if (is_recently_opened != 0) {is_recently_opened = 0;}
        is_saved = 0;
        insert_char(gap_buffer, ch, gap_buffer_cursor_1dim_position);
        gap_buffer_cursor_1dim_position++;
        screen_1dim_rel_pos++;
        calculate_screen_1dim_pos(gap_buffer, base_1dim_pos, screen_1dim_rel_pos, &calculated_screen_1dim_pos);
        break;
    }
}

int main(int argc, char* argv[]) {

    #if defined(__APPLE__) || defined(__linux__)
        system("stty -ixon");
    #endif

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Text Area
    WINDOW* text_window = newwin(LINES - 2, COLS, 0, 0);

    // Status Area
    WINDOW* status_window = newwin(1, COLS, LINES - 2, 0);

    // Message Area
    WINDOW* message_window = newwin(1, COLS, LINES - 2 + 1, 0);

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
        open_file(gap_buffer, file_name, file_extension);
        total_lines = get_total_lines(gap_buffer);
        free(filename_buffer);
    }
    else {
        strcpy(file_name, "\0");
        strcpy(file_extension, "\0");
    }

    while (1) {
        werase(text_window);
        handle_key_(text_window, status_window, message_window, gap_buffer);
        calculate_screen_1dim_pos(gap_buffer, base_1dim_pos, screen_1dim_rel_pos, &calculated_screen_1dim_pos);
        if(gap_buffer_cursor_1dim_position < base_1dim_pos) {
            base_1dim_pos = 0;
            screen_1dim_rel_pos = gap_buffer_cursor_1dim_position - base_1dim_pos;
            calculate_screen_1dim_pos(gap_buffer, base_1dim_pos, screen_1dim_rel_pos, &calculated_screen_1dim_pos);
            scr_y_pos_2dim = calculated_screen_1dim_pos / COLS;
            scr_x_pos_2dim = calculated_screen_1dim_pos % COLS;
            move(scr_y_pos_2dim, scr_x_pos_2dim);
            werase(text_window);
            refresh();
        }
        if (calculated_screen_1dim_pos >= (COLS*(LINES-2))) {
            base_1dim_pos = gap_buffer_cursor_1dim_position - 1;
            screen_1dim_rel_pos = gap_buffer_cursor_1dim_position - base_1dim_pos;
            calculate_screen_1dim_pos(gap_buffer, base_1dim_pos, screen_1dim_rel_pos, &calculated_screen_1dim_pos);
            scr_y_pos_2dim = calculated_screen_1dim_pos / COLS;
            scr_x_pos_2dim = calculated_screen_1dim_pos % COLS;
            move(scr_y_pos_2dim, scr_x_pos_2dim);
            werase(text_window);
            refresh();
        }else
        scr_y_pos_2dim = calculated_screen_1dim_pos / COLS;
        scr_x_pos_2dim = calculated_screen_1dim_pos % COLS;
        move(scr_y_pos_2dim, scr_x_pos_2dim);
        draw_status_bar(COLS, file_name, file_extension, base_1dim_pos, gap_buffer_cursor_1dim_position, status_window);
        //draw_status_bar(COLS, file_name, file_extension, scr_csr_x, scr_csr_y, status_window);

        draw_text_area(text_window, gap_buffer, base_1dim_pos);
        refresh_screen(text_window, status_window, message_window);

        }

}
