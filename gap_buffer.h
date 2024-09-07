#ifndef gap_buffer_h
#define gap_buffer_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char* char_buffer;
	int size;
	int gap_start;
	int gap_end;
}GapBuffer;

GapBuffer* create_gap_buffer(int size);
GapBuffer* resize_gap_buffer(GapBuffer* gap_buffer);
GapBuffer* sync_cursor_pos_with_buffer_size(GapBuffer* gap_buffer, int pos);
void insert_char(GapBuffer* gap_buffer, char c, int pos);
void delete_char(GapBuffer* gap_buffer, int pos);
#endif