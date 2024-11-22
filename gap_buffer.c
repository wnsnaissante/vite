#include "gap_buffer.h"
#include "terminal.h"


GapBuffer* create_gap_buffer(int size) {
	GapBuffer* gap_buffer = (GapBuffer*)malloc(sizeof(GapBuffer));
	gap_buffer->char_buffer = (char*)calloc(size, sizeof(char));
    gap_buffer->size = size;
    gap_buffer->gap_start = 0;
    gap_buffer->gap_end = size - 1;
	return gap_buffer;
}

GapBuffer* resize_gap_buffer(GapBuffer* gap_buffer) {
    size_t before_realloc_size = gap_buffer->size;
    size_t new_alloc_size;

    if (before_realloc_size >> 2 < 9) {
        new_alloc_size = before_realloc_size + 3;
    }
    else {
        new_alloc_size = before_realloc_size + (before_realloc_size >> 1) + 6;
    }

    // Align Memory to multiples of 4 
    new_alloc_size = (new_alloc_size + 3) & ~3;

    // Allocate new buffer
    char* new_buffer = (char*)realloc(gap_buffer->char_buffer, new_alloc_size * sizeof(char));
    
    gap_buffer->char_buffer = new_buffer;

    gap_buffer->size = new_alloc_size;
    gap_buffer->gap_end = new_alloc_size - 1;

    return gap_buffer;
}


GapBuffer* move_gap_left(GapBuffer* gap_buffer, int position) {
    while (gap_buffer->gap_start > position) {
        gap_buffer->char_buffer[gap_buffer->gap_end] = gap_buffer->char_buffer[gap_buffer->gap_start - 1];
        gap_buffer->char_buffer[gap_buffer->gap_start - 1] = '\0';
        gap_buffer->gap_start--;
        gap_buffer->gap_end--;
    }
    return gap_buffer;
}

GapBuffer* move_gap_right(GapBuffer* gap_buffer, int position) {
    while (gap_buffer->gap_start < position) {
        gap_buffer->char_buffer[gap_buffer->gap_start] = gap_buffer->char_buffer[gap_buffer->gap_end + 1];
        gap_buffer->char_buffer[gap_buffer->gap_end + 1] = '\0';
        gap_buffer->gap_start++;
        gap_buffer->gap_end++;
    }
    return gap_buffer;
}


void move_buf_cursor(GapBuffer* gap_buffer, int position) {
    if (position < gap_buffer->gap_start) {
        move_gap_left(gap_buffer, position);
    }
    else
    {
        move_gap_right(gap_buffer, position);
    }
}

void insert_char(GapBuffer* gap_buffer, char c, int position) {
    if (position != gap_buffer->gap_start) {
        move_buf_cursor(gap_buffer, position);
    }

    if (gap_buffer->gap_start == gap_buffer->gap_end) {
        gap_buffer = resize_gap_buffer(gap_buffer);
    }

    gap_buffer->char_buffer[gap_buffer->gap_start] = c;
    gap_buffer->gap_start++;
}

void delete_char(GapBuffer* gap_buffer, int position) {
    if (position != gap_buffer->gap_start) {
        move_buf_cursor(gap_buffer, position);
    }

    if (gap_buffer->gap_start > 0) {
        gap_buffer->gap_start--;
        memset(&gap_buffer->char_buffer[gap_buffer->gap_start], (char)0, sizeof(char) * 1);
    }
}
int get_above_line_last_index(GapBuffer* gap_buffer, int position) {
    int result = 0;
    for (int i = 0; i <= position; i++)
    {
        if (gap_buffer->char_buffer[i] == '\n') {
            result = i;
        }
    }
    return result;
}

void calc_last_line_change_position(GapBuffer* gap_buffer, int position, int* x, int* y) {
    int target = position;
    int target_x = 0;
    int target_y = 0;
    for (int i = 0; i < target-1; i++)
    {
        target_x++;
        if (target_x >= COLS || gap_buffer->char_buffer[i]=='\n') {
            target_y++;
            target_x = 0;
        }
    }
    *x = target_x;
    *y = target_y;
}

int get_total_lines(GapBuffer* gap_buffer) {
    int total_lines = 1;
    for (int i = 0; i < gap_buffer->size; i++)
    {
        if (gap_buffer->char_buffer[i] == '\n') {
            total_lines++;
        }
    }
    return total_lines;
}

int calc_page(int* y) {
    return *y / (LINES-2);
}

void save_to_file(GapBuffer* gap_buffer, const char* filename, const char* file_extension) {
    char target[FILENAME_MAX];
    if (file_extension == NULL) {
        snprintf(target, sizeof(target), "%s", filename);
    }
    else {
        snprintf(target, sizeof(target), "%s%s", filename, file_extension);
    }
    
    FILE* file = fopen(target, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    fwrite(gap_buffer->char_buffer, sizeof(char), gap_buffer->gap_start, file);

    fwrite(gap_buffer->char_buffer + gap_buffer->gap_end + 1, sizeof(char),
        gap_buffer->size - gap_buffer->gap_end - 1, file);

    fclose(file);
}

int open_file(GapBuffer *gap_buffer, const char *filename, const char *file_extension) {
	char target[FILENAME_MAX];
	snprintf(target, sizeof(target), "%s%s", filename, file_extension);
	FILE* file = fopen(target, "r");
	if (file == NULL) {
		perror("Failed to open file");
		return;
	}
    char c;
	int position = 0;
    while ((c = fgetc(file)) != EOF) {
        insert_char(gap_buffer, c, position);
        position++;
    }
    fclose(file);
    return gap_buffer->gap_start;
}