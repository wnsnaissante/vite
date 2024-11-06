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

char file_name[100];
char file_extension[10];

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
            break;
        case 6: // Ctrl-F
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
            delete_char(gap_buffer, 0);
		    break;
	    case KEY_HOME:
		    break;
	    case KEY_END:
		    break;
	    default:
            insert_char(gap_buffer, ch, 0);
		    break;
        }
    draw_default_message_bar(message_window);
    draw_status_bar(COLS, file_name, file_extension, 0, 0, status_window);
    draw_text_area(text_window, gap_buffer);
    refresh_screen(text_window, status_window, message_window);
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
        strcpy(file_name, "No Name");
        strcpy(file_extension, "no tf");
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
    }
    return 0;
}

