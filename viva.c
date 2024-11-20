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
#include "terminal.h"
#include "gap_buffer.h"

static char file_name[100];
static char file_extension[10];

static int scr_csr_x = 0;
static int scr_csr_y = 0;
static int gap_buffer_cursor_1d_position = 0;

void handle_key_(WINDOW* text_window, WINDOW* status_window, WINDOW* message_window, GapBuffer* gap_buffer) {
    int ch = getch();
    switch (ch) {
        case 17: // Ctrl-Q
            if (strlen(gap_buffer->char_buffer)==0) {
                endwin(text_window);
                endwin(status_window);
                endwin(message_window);
				exit(0);
                break;
			}
			else {
				draw_quit_message_bar(message_window);
                refresh_screen(text_window, status_window, message_window);
                ch = getch();
                if (ch == 17) {
                    endwin(text_window);
                    endwin(status_window);
                    endwin(message_window);
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
        case 19: // Ctrl-S
            save_to_file(gap_buffer, file_name, file_extension);
            break;
        case 6: // Ctrl-F
            break;
        case KEY_UP:
		    break;
        case KEY_DOWN:
            break;
        case KEY_LEFT:
            scr_csr_x--;
            gap_buffer_cursor_1d_position--;
            move(scr_csr_y, scr_csr_x);
            break;
	    case KEY_RIGHT:
            scr_csr_x++;
            move(scr_csr_y, scr_csr_x);
            break;
        case KEY_BACKSPACE:
            break;
        case 8: // Backspace
            if (scr_csr_x >= 1) {
                delete_char(gap_buffer, gap_buffer_cursor_1d_position);
                scr_csr_x--;
                gap_buffer_cursor_1d_position--;
                move(scr_csr_y, scr_csr_x);
                break;
            }
            else {
                
            }
            break;
	    case 449: // 노트북 홈키
		    break;
	    case KEY_END:
		    break;
	    default:
            insert_char(gap_buffer, ch, gap_buffer_cursor_1d_position);
            gap_buffer_cursor_1d_position++;
            if (ch == 10) {
                scr_csr_x = 0;
                scr_csr_y++;

                move(scr_csr_y, scr_csr_x);
            }
            else {
                scr_csr_x++;

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

    GapBuffer* gap_buffer = create_gap_buffer(1024);
    int gap_buffer_cursor = 0;
    if (argc > 1)   // get file name and its extension from parameter
    {
        strcpy(file_name, argv[1]);
        strcpy(file_extension, strpbrk(file_name, "."));
    }
    else {
        strcpy(file_name, "test_filesave");
        strcpy(file_extension, ".txt");
    }
    

    int height = LINES - 2;
    int width = COLS;

    // Text Area
    WINDOW* text_window = newwin(height, width, 0, 0);

    // Status Area
    WINDOW* status_window = newwin(1, width, height, 0);

    // Message Area
    WINDOW* message_window = newwin(1, width, height + 1, 0);

    while (1) {
        handle_key_(text_window, status_window, message_window, gap_buffer);
        werase(text_window);
        draw_default_message_bar(message_window);
        //draw_status_bar(COLS, file_name, file_extension, scr_csr_y, get_total_lines(gap_buffer), status_window);
        draw_status_bar(COLS, file_name, file_extension, gap_buffer_cursor_1d_position, scr_csr_y, status_window);
        waddstr(text_window, gap_buffer->char_buffer);
        refresh_screen(text_window, status_window, message_window);
    }
    return 0;
}

