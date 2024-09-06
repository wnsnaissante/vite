#include "gap_buffer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

GapBuffer* create_gap_buffer(int size) {
	GapBuffer* gap_buffer = (GapBuffer*)malloc(sizeof(GapBuffer));
	gap_buffer->char_buffer = (char*)malloc(sizeof(char)*size);
	gap_buffer->size_of_gap = size;
	gap_buffer->gap_start = 0;
	gap_buffer->gap_end = size - 1;
	return gap_buffer;
}
