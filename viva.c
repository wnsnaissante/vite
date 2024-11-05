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

int main(int argc, char* argv[]) {
    char file_name[100];
    char file_extension[10];
    if (argc > 1)   // get file name and its extension from parameter
    {
        strcpy(file_name, argv[1]);
        strcpy(file_extension, strpbrk(file_name, "."));
    }
    else {
        strcpy(file_name, "No Name");
        strcpy(file_extension, "no tf");
    }
    initscr();
    cbreak();
    noecho();

    int height = LINES - 2;
    int width = COLS;

    // Text Area
    WINDOW* text_window = newwin(height, width, 0, 0);

    // Status Area
    WINDOW* status_window = newwin(1, width, height, 0);

    // Message Area
    WINDOW* message_window = newwin(1, width, height + 1, 0);

    while (1) {
        draw_status_bar(width, file_name, file_extension,0,0, status_window);
        draw_message_bar(message_window);
        refresh_screen(text_window, status_window, message_window);
    }
    return 0;
}