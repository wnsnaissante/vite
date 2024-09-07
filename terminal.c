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

int is_ctrl_pressed(DWORD controlKeyState) {
	return (controlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0;
}

void handle_key_input(INPUT_RECORD inputRecord, Cursor* cursor, GapBuffer* gap_buffer, int terminal_width, int last_line_no) {
	int pos = flatten_cursor_position(cursor, terminal_width);
	if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {
		KEY_EVENT_RECORD keyEvent = inputRecord.Event.KeyEvent;

		if (keyEvent.uChar.AsciiChar != 0) {
			if (pos < gap_buffer->size)
			{
				gap_buffer = sync_cursor_pos_with_buffer_size(gap_buffer, pos);
			}
			insert_char(gap_buffer, keyEvent.uChar.AsciiChar, pos);
			return;
		}
		
		WORD keyCode = inputRecord.Event.KeyEvent.wVirtualKeyCode;
		switch (keyCode) {
			case VK_BACK:
				delete_char(gap_buffer, pos);
				break;
			case VK_HOME:
				printf("%s\n%d %d", gap_buffer->char_buffer,gap_buffer->gap_start,gap_buffer->gap_end);
				break;
			case VK_END:
			case VK_PRIOR:  // Page Up
			case VK_NEXT:  // Page Down
			case VK_UP:    // Arrow Up
			case VK_DOWN:  // Arrow Down
			case VK_LEFT:  // Arrow Left
			case VK_RIGHT: // Arrow Right
			case VK_SPACE:
				break;
		}
	}
	return;
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
