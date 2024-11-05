#include "terminal.h"

void draw_text_area(WINDOW* text_window) {

}

void draw_status_bar(int width, char* file_name, char* file_extension, int current_line, int total_lines, WINDOW* status_window) {
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    wattron(status_window, COLOR_PAIR(1));

    int blanks_count = width - (int)strlen(file_name) - (int)strlen(file_extension) - 20;
    char* status = (char*)malloc(width + 1);
    
    snprintf(status, width + 1, "[%s] - %d line %*s %s | %d/%d",
        file_name, current_line,
        width - (int)strlen(file_name) - (int)strlen(file_extension) - 20, "",
        file_extension, current_line, total_lines);

    mvwprintw(status_window, 0, 0, "%s", status);
    wattroff(status_window, COLOR_PAIR(1));
}

void draw_message_bar(WINDOW* message_window) {
	mvwprintw(message_window, 0, 0, "HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");
}

void refresh_screen(WINDOW* text_window, WINDOW* status_window, WINDOW* message_window) {
	wrefresh(text_window);
	wrefresh(status_window);
	wrefresh(message_window);
}

