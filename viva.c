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
#include <stdlib.h>
#include <string.h>
#include "terminal.h"


int main(int argc, char* argv[]) {
#ifdef _WIN32   // Init Terminal Raw Mode
    HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
    DWORD crntMode;
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
    

    int max_rows, max_cols;
    get_console_size(&max_rows, &max_cols);

    Cursor* usr_cursor = create_new_cursor();

    INPUT_RECORD inputRecord;
    DWORD events;
    while (1)
    {
        ReadConsoleInput(hConsole, &inputRecord, 1, &events);
        if (inputRecord.EventType == KEY_EVENT) {
            char c = process_key_events(inputRecord.Event.KeyEvent, usr_cursor);
            if (c != 0) {
                printf("You pressed: %c\n", c);
            }
        }
    }
    
    
    return 0;
}