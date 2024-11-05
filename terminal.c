#include "terminal.h"

Cursor* create_new_cursor() {
	Cursor* cursor = (Cursor*)malloc(sizeof(Cursor));
	cursor->buffer_pos = 0;
	cursor->virtual_row = 1;
	cursor->virtual_col = 1;
	return cursor;
}

int flatten_cursor_pos(Cursor* cursor, int terminal_width) {
	return (cursor->virtual_row - 1 * terminal_width) + cursor->virtual_col - 1;
}

void buffer_idx_to_cursor_pos(Cursor* cursor, int terminal_width) {
	cursor->virtual_row	= cursor->buffer_pos / terminal_width;
	cursor->virtual_col = cursor->buffer_pos % terminal_width;
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


void handle_key(Cursor* cursor, GapBuffer* gap_buffer, int terminal_height, int terminal_width, int* scroll_offset) {
	int key = _getch();
	switch (key) {
	case 0xE0:
		key = _getch();
		switch (key) {
		case 0x48:	// Up arrow
			break;
		case 0x50: // Down arrow
			break;
		case 0x4B: // Left arrow
			cursor->buffer_pos--;
			break;
		case 0x4D: // Right arrow
			
			break;
		case 0x49: // Page up 
			break;
		case 0x51: // Page down
			break;
		case 0x47: printf("HOME key pressed\n"); break;
		case 0x4F: printf("END key pressed\n"); break;
		}
	case 6: // ctrl + f | FIND
		break;
	case 17: // ctrl + q | EXIT
		exit(0);
		break;
	case 19: // ctrl + s | SAVE
		break;
	case 8: // Backspace

		break;
	case 13: // Enter key
		
		break;
	default: // Regular Keys
		insert_char(gap_buffer, (char)key, 0);
		break;
	}
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
