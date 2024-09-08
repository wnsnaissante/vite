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

void print_gap_buffer(GapBuffer* gap_buffer) {
    clear_screen();
    for (int i = 0; i < gap_buffer->size; i++)
    {
        if (gap_buffer->char_buffer[i] == 0) continue;
        else
        {
            printf("%c", gap_buffer->char_buffer[i]);
        }
    }
}

GapBuffer* sync_buffer_size_with_cursor_pos(GapBuffer* gap_buffer, int pos) {
    if (pos > gap_buffer->size) {
        while (gap_buffer->size < pos) {
            gap_buffer = resize_gap_buffer(gap_buffer);
        }
    }
    return gap_buffer;
}
GapBuffer* resize_gap_buffer(GapBuffer* gap_buffer) {
    size_t before_realloc_size = gap_buffer->size;
    size_t additional_size;
    if (before_realloc_size < 9) {
        additional_size = 3;
    }
    else {
        additional_size = 6;
    }
    size_t new_gap_buffer_size = before_realloc_size + (before_realloc_size / 3) + additional_size;

    char* new_buffer = (char*)calloc(new_gap_buffer_size, sizeof(char));
    memcpy(new_buffer, gap_buffer->char_buffer, before_realloc_size);
    gap_buffer->gap_end = new_gap_buffer_size - 1;
    if (gap_buffer->gap_end >= new_gap_buffer_size) {
        gap_buffer->gap_end = new_gap_buffer_size - 1;
    }
    free(gap_buffer->char_buffer);
    gap_buffer->char_buffer = new_buffer;
    gap_buffer->size = new_gap_buffer_size;

    return gap_buffer;
}

void move_gap_left(GapBuffer* gap_buffer, int position) {
    if (position < gap_buffer->gap_start || position > gap_buffer->gap_end) {
        return;
    }
    while (position < gap_buffer->gap_start)
    {
        if (gap_buffer->gap_start == 0) {
            break;  
        }
        gap_buffer->gap_start--;
        gap_buffer->gap_end--;
        memset(&gap_buffer->char_buffer[gap_buffer->gap_end + 1], gap_buffer->char_buffer[gap_buffer->gap_start],1*sizeof(char));
        memset(&gap_buffer->char_buffer[gap_buffer->gap_start], 0, 1 * sizeof(char));
    }
}

void move_gap_right(GapBuffer* gap_buffer, int position) {
    if (position < gap_buffer->gap_start || position > gap_buffer->gap_end) {
        return;
    }
    while (position > gap_buffer->gap_start) {
        if (gap_buffer->gap_end >= gap_buffer->size - 1) {
            gap_buffer = resize_gap_buffer(gap_buffer);
        }
        gap_buffer->gap_start++;
        gap_buffer->gap_end++;

        memset(&gap_buffer->char_buffer[gap_buffer->gap_start - 1], gap_buffer->char_buffer[gap_buffer->gap_end], 1 * sizeof(char));
        memset(&gap_buffer->char_buffer[gap_buffer->gap_end], 0, 1 * sizeof(char));
    }
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

    if (gap_buffer->gap_start < gap_buffer->size) {
        memset(&gap_buffer->char_buffer[gap_buffer->gap_start], c, sizeof(char) * 1);
        gap_buffer->gap_start++;
    }
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
