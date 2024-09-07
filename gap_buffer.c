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

GapBuffer* sync_cursor_pos_with_buffer_size(GapBuffer* gap_buffer, int pos) {
    GapBuffer* temp = 0;
    if (pos > gap_buffer->size)
    {
        while (gap_buffer->size < pos) {
            temp = resize_gap_buffer(gap_buffer);
        }
    }
    else
    {
        temp = gap_buffer;
    }
    return temp;
}

void insert_char(GapBuffer* gap_buffer, char c, int pos) {
    if (gap_buffer->gap_start == gap_buffer->gap_end)
    {
        resize_gap_buffer(gap_buffer);
    }

    gap_buffer->char_buffer[gap_buffer->gap_start] = c;
    gap_buffer->gap_start++;
}

void delete_char(GapBuffer* gap_buffer, int pos) {
    if (gap_buffer->gap_start == gap_buffer->gap_end) {
        resize_gap_buffer(gap_buffer);
    }
    gap_buffer->char_buffer[pos] = 0;
    gap_buffer->gap_start--;
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
    free(gap_buffer->char_buffer);
    gap_buffer->char_buffer = new_buffer;
    gap_buffer->size = new_gap_buffer_size;

    return gap_buffer;
}
