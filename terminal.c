#include "terminal.h"

Cursor* create_new_cursor() {
    Cursor* cursor = (Cursor*)malloc(sizeof(Cursor));
    cursor->row = 1;
    cursor->col = 1;
    return cursor;
}

void calc_cursor_movement() {

}

#ifdef _WIN32
void get_console_size(int* rows, int* cols) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    *cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void enable_raw_mode(HANDLE hConsole, DWORD* crntMode) {
    DWORD mode;
    GetConsoleMode(hConsole, crntMode);
    mode = *crntMode;
    mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    SetConsoleMode(hConsole, mode);
}

void process_special_key_events(WORD virtualKeyCode,Cursor* cursor) {
    switch (virtualKeyCode) {
        case VK_BACK:
            break;
        case VK_HOME:
            cursor->row = 1;
            break;
        case VK_END:

            break;
        case VK_PRIOR:  // Page Up
            
            break;
        case VK_NEXT:  // Page Down
            
            break;
        case VK_UP:
            
            break;
        case VK_DOWN:

            break;
        case VK_LEFT:

            break;
        case VK_RIGHT:

            break;
        default:
            break;
    }
}

char process_key_events(KEY_EVENT_RECORD keyEvent,Cursor* cursor) {
    if (keyEvent.bKeyDown) {
        switch (keyEvent.wVirtualKeyCode) {
        case VK_BACK:
        case VK_HOME:
        case VK_END:
        case VK_PRIOR:  // Page Up
        case VK_NEXT:  // Page Down
        case VK_UP:    // Arrow Up
        case VK_DOWN:  // Arrow Down
        case VK_LEFT:  // Arrow Left
        case VK_RIGHT: // Arrow Right
            process_special_key_events(keyEvent.wVirtualKeyCode, cursor);
            return 0;  // 특수 키는 0을 리턴
        default:
            if (keyEvent.uChar.AsciiChar != 0) {
                return keyEvent.uChar.AsciiChar;  // 일반 키는 해당 문자 리턴
            }
            break;
        }
    }
    return 0;
}
#else
void get_console_size(int* rows, int* cols) {
    struct winsize console_size;
    *rows = console_size.ws_row;
    *cols = console_size.ws_col;
}

void enable_raw_mode(struct termios* orig_termios) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, orig_termios);
    raw = *orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
#endif



void move_cursor(int row, int col) {
    printf("\033[%d;%dH", row, col);
}

void set_background_color(int color) {
    printf("\033[48;5;%dm", color);
}

void set_text_color(int color) {
    printf("\033[38;5;%dm", color);
}

void reset_colors() {
    printf("\033[0m");
}

void clear_screen() {
    printf("\033[2J");
    printf("\033[H");
}
