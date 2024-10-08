#include "terminal.h"

Cursor* create_new_cursor() {
	Cursor* cursor = (Cursor*)malloc(sizeof(Cursor));
	cursor->row = 1;
	cursor->col = 1;
	return cursor;
}

// Flatten the cursor's 2-dimensional position to a 1-dimensional position
int flatten_cursor_position(Cursor* cursor, int terminal_width) {
	int index = (cursor->row - 1) * terminal_width + (cursor->col - 1);
	return index;
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


void handle_key(Cursor* buf_cursor, Cursor* scr_cursor, GapBuffer* gap_buffer, int terminal_height, int terminal_width) {
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
			delete_char(gap_buffer, flatten_cursor_position(buf_cursor, terminal_width));
			if (buf_cursor->col == 1)
			{
				if (buf_cursor->row <= 1)
				{
					break;
				}
				buf_cursor->col = terminal_width;
				buf_cursor->row--;
			}
			else buf_cursor->col--;
			break;
		case 13: // Enter key
			break;
		default: // Regular Keys
			insert_char(gap_buffer, (char)key, flatten_cursor_position(buf_cursor, terminal_width));
			if (buf_cursor->col < terminal_width)
			{
				buf_cursor->col++;
			}
			else {
				buf_cursor->col -= terminal_width - 1;
				buf_cursor->row++;
			}
			if (flatten_cursor_position(buf_cursor, terminal_width) >= gap_buffer->size)
			{
				gap_buffer = resize_gap_buffer(gap_buffer);
			}
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

void calc_cursor_pos(GapBuffer* gap_buffer) {

}


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

void draw_screen() {
	clear_screen();
}

void draw_context() {

}

void draw_status_bar() { 

}

void draw_message_bar() {

}

