#include "terminal.h"
#include "gap_buffer.h"

Cursor* create_new_cursor() {
	Cursor* cursor = (Cursor*)malloc(sizeof(Cursor));
	cursor->row = 1;
	cursor->col = 1;
	return cursor;
}

void calc_cursor_movement(Cursor* cursor, WORD KeyCode, int last_line, int max_row_size, int max_col_size) {
	switch (KeyCode) {
	case VK_BACK:
		if (cursor->row == 1 && cursor->col == 1) {
			break;
		}
		if (cursor->col <= 1)
		{
			cursor->row -= 1;
			cursor->col = max_col_size;
			break;
		}
		else
		{
			cursor->col -= 1;
		}
		break;
	case VK_HOME:
		cursor->row = 1;
		break;
	case VK_END:
		cursor->row = last_line;
		break;
	case VK_PRIOR:  // Page Up
		if (cursor->row == 1)
		{
			break;
		}
		cursor->row -= max_row_size;
		break;
	case VK_NEXT:  // Page Down
		cursor->row += max_row_size;
		break;
	case VK_UP:
		if (cursor->row == 1)
		{
			break;
		}
		else {
			cursor->row--;
		}
		break;
	case VK_DOWN:
		cursor->row++;
		break;
	case VK_LEFT:
		if (cursor->row == 1 && cursor->col == 1) {
			break;
		}
		if (cursor->col <= 1)
		{
			cursor->row -= 1;
			cursor->col = max_col_size;
			break;
		}
		else
		{
			cursor->col -= 1;
		}
		break;
	case VK_RIGHT:
		if (cursor->col >= max_col_size)
		{
			cursor->row += 1;
			cursor->col -= max_col_size-1;
		}
		else
		{
			cursor->col += 1;
		}
		break;
	case VK_SPACE:
		if (cursor->col >= max_col_size)
		{
			cursor->col -= max_col_size - 1;
			cursor->row += 1;
		}
		cursor->col += 1;
		break;
	}
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

void process_special_key_events(WORD virtualKeyCode,Cursor* cursor, int max_row_size, int last_line, int max_col_size) {
	switch (virtualKeyCode) {
		case VK_BACK:
			calc_cursor_movement(cursor, VK_BACK, last_line, max_row_size, max_col_size);
			break;
		case VK_HOME:
			calc_cursor_movement(cursor, VK_HOME, last_line, max_row_size, max_col_size);
			break;
		case VK_END:
			calc_cursor_movement(cursor, VK_END, last_line, max_row_size, max_col_size);
			break;
		case VK_PRIOR:  // Page Up
			calc_cursor_movement(cursor, VK_PRIOR, last_line, max_row_size, max_col_size);
			break;
		case VK_NEXT:  // Page Down
			calc_cursor_movement(cursor, VK_NEXT, last_line, max_row_size, max_col_size);
			break;
		case VK_UP:
			calc_cursor_movement(cursor, VK_UP, last_line, max_row_size, max_col_size);
			break;
		case VK_DOWN:
			calc_cursor_movement(cursor, VK_DOWN, last_line, max_row_size, max_col_size);
			break;
		case VK_LEFT:
			calc_cursor_movement(cursor, VK_LEFT, last_line, max_row_size, max_col_size);
			break;
		case VK_RIGHT:
			calc_cursor_movement(cursor, VK_RIGHT, last_line, max_row_size, max_col_size);
			break;
		case VK_SPACE:
			calc_cursor_movement(cursor, VK_SPACE, last_line, max_row_size, max_col_size);
			break;
		default:
			break;
	}
}

char process_key_events(KEY_EVENT_RECORD keyEvent,Cursor* cursor, int max_row_size, int last_line, int max_col_size) {
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
		case VK_SPACE:
			process_special_key_events(keyEvent.wVirtualKeyCode, cursor, max_row_size, last_line, max_col_size);
			return 0;  // 
		default:
			if (keyEvent.uChar.AsciiChar != 0) {
				return keyEvent.uChar.AsciiChar;
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
