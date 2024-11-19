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

void insert_char(GapBuffer* gap_buffer, char c, int position);
void delete_char(GapBuffer* gap_buffer, int position);
GapBuffer* create_gap_buffer(int size);
int get_above_line_last_index(GapBuffer* gap_buffer, int position);
void calc_last_line_change_position(GapBuffer* gap_buffer, int position, int* x, int* y);
int get_total_lines(GapBuffer* gap_buffer);
void save_to_file(GapBuffer* gap_buffer, const char* filename, const char* file_extension);

#endif