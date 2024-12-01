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
    size_t previous_size = gap_buffer->size;

    // Align Memory to multiples of 4
    size_t expanded_size = ((int)(previous_size * 1.25) + 3) & ~3;

    // Allocate new buffer
    char* new_buffer = (char*)realloc(gap_buffer->char_buffer, expanded_size * sizeof(char));
    memset(new_buffer+previous_size, '\0', (expanded_size-previous_size) * sizeof(char));
    gap_buffer->char_buffer = new_buffer;

    gap_buffer->size = expanded_size;
    gap_buffer->gap_end = expanded_size - 1;

    return gap_buffer;
}

GapBuffer* move_gap_left(GapBuffer* gap_buffer, int position) {
    int shift_distance = gap_buffer->gap_start - position;

    if (shift_distance > 0) {
        if (position < 0 || position > gap_buffer->gap_start) {
            return gap_buffer;
        }

        memmove(
            gap_buffer->char_buffer + gap_buffer->gap_end - shift_distance,
            gap_buffer->char_buffer + gap_buffer->gap_start - shift_distance,
            shift_distance
        );

        gap_buffer->gap_start -= shift_distance;
        gap_buffer->gap_end -= shift_distance;
    }

    return gap_buffer;
}

GapBuffer* move_gap_right(GapBuffer* gap_buffer, int position) {
    int shift_distance = position - gap_buffer->gap_start;
    if (shift_distance > 0) {
        if (position < gap_buffer->gap_start || position >= gap_buffer->size) {
            return gap_buffer;
        }

        memmove(
            gap_buffer->char_buffer + gap_buffer->gap_start,
            gap_buffer->char_buffer + gap_buffer->gap_end + 1,
            shift_distance
        );

        gap_buffer->gap_start += shift_distance;
        gap_buffer->gap_end += shift_distance;
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

void insert_char(GapBuffer* gap_buffer, char char_to_insert, int position) {
    if (position != gap_buffer->gap_start) {
        move_buf_cursor(gap_buffer, position);
    }

    if (gap_buffer->gap_start == gap_buffer->gap_end) {
        gap_buffer = resize_gap_buffer(gap_buffer);
    }

    gap_buffer->char_buffer[gap_buffer->gap_start] = char_to_insert;
    gap_buffer->gap_start++;
}

void delete_char(GapBuffer* gap_buffer, int position) {
    if (position != gap_buffer->gap_start) {
        move_buf_cursor(gap_buffer, position);
    }

    if (gap_buffer->gap_start > 0) {
        gap_buffer->gap_start--;
        memset(&gap_buffer->char_buffer[gap_buffer->gap_start], (char)'\0', sizeof(char) * 1);
    }
}

int get_total_lines(GapBuffer* gap_buffer) {
    int total_lines = 1;
    int current_col = 0;

    for (int i = 0; i < gap_buffer->size; i++) {
        char parsed_char = gap_buffer->char_buffer[i];
        if (parsed_char == '\n') {
            total_lines++;
            current_col = 0;
        } else if (parsed_char != '\0') {
            current_col++;
            if (current_col >= COLS) {
                total_lines++;
                current_col = 0;
            }
        }
    }

    return total_lines;
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
        return;
    }

    fwrite(gap_buffer->char_buffer, sizeof(char), gap_buffer->gap_start, file);

    fwrite(gap_buffer->char_buffer + gap_buffer->gap_end + 1, sizeof(char),
        gap_buffer->size - gap_buffer->gap_end - 1, file);

    fclose(file);
}

void open_file(GapBuffer *gap_buffer, const char *filename, const char *file_extension) {
    char target[FILENAME_MAX];
    if (file_extension != '\0') {
        snprintf(target, sizeof(target), "%s%s", filename, file_extension);
    }
    else {
        snprintf(target, sizeof(target), "%s", filename);
    }

    FILE* file = fopen(target, "r");
    if (file == NULL) {
        exit(404);
    }
    char parsed_char;
    int position = 0;
    while ((parsed_char = fgetc(file)) != EOF) {
        insert_char(gap_buffer, parsed_char, position);
        position++;
    }
    fclose(file);
}