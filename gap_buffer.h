#pragma once
typedef struct {
	char* char_buffer;
	int size_of_gap;
	int gap_start;
	int gap_end;
}GapBuffer;

GapBuffer* create_gap_buffer(int size);
