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
#ifdef _WIN32   // Init Terminal Raw Mode
    HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
    DWORD crntMode;
    INPUT_RECORD inputRecord;
    DWORD events;
    enable_raw_mode(hConsole, &crntMode);
#else
    struct termios orig_termios;
    enable_raw_mode(&orig_termios);
#endif

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

    int terminal_height, terminal_width;
    int last_line = 1;
    get_console_size(&terminal_height, &terminal_width);

    GapBuffer* gap_buf = create_gap_buffer(5242880); // About 5MB zz 5242880

    Cursor* usr_cursor = create_new_cursor();

    sync_cursor_pos_with_buffer_size(gap_buf, flatten_cursor_position(usr_cursor, terminal_width));
    printf("%d %d %d \n", gap_buf->size,gap_buf->gap_start, gap_buf->gap_end);
    while (1)
    {
        ReadConsoleInput(hConsole, &inputRecord, 1, &events);
        if (inputRecord.EventType == KEY_EVENT) {
            handle_key_input(inputRecord, usr_cursor, gap_buf, terminal_width, 1);
        }
    }
    return 0;
}