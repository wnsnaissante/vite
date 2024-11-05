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


#endif