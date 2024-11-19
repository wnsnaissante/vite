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
    printf("%d %d\n", before_realloc_size, new_alloc_size);

    // Allocate new buffer
    char* new_buffer = (char*)calloc(new_alloc_size, sizeof(char));
    memcpy(new_buffer, gap_buffer->char_buffer, before_realloc_size);


    free(gap_buffer->char_buffer);
    gap_buffer->char_buffer = new_buffer;
    gap_buffer->size = new_alloc_size;
    gap_buffer->gap_end = new_alloc_size - 1;

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
        memset(&gap_buffer->char_buffer[gap_buffer->gap_end + 1], gap_buffer->char_buffer[gap_buffer->gap_start], 1 * sizeof(char));
        memset(&gap_buffer->char_buffer[gap_buffer->gap_start], 0, 1 * sizeof(char));
    }
}

GapBuffer* move_gap_right(GapBuffer* gap_buffer, int position) {
    // 갭이 이동할 필요가 없으면 반환
    if (position <= gap_buffer->gap_start) {
        return gap_buffer;
    }

    // 이동 거리 계산
    int move_distance = position - gap_buffer->gap_start;

    // 갭을 이동하기 전에 크기 조정
    if (gap_buffer->gap_end + move_distance >= gap_buffer->size) {
        gap_buffer = resize_gap_buffer(gap_buffer);
    }

    // 갭을 오른쪽으로 이동
    memmove(&gap_buffer->char_buffer[gap_buffer->gap_start],
        &gap_buffer->char_buffer[gap_buffer->gap_end],
        move_distance);

    // 갭 인덱스 업데이트
    gap_buffer->gap_start += move_distance;
    gap_buffer->gap_end += move_distance;

    // 갭을 비우기
    memset(&gap_buffer->char_buffer[gap_buffer->gap_start], '\0', move_distance);

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
    for (int i = 0; i < target; i++)
    {
        target_x++;
        if (target_x > COLS || gap_buffer->char_buffer[i]=='\n') {
            target_y++;
            target_x = 0;
        }
    }
    *x = target_x;
    *y = target_y + 1;
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

void save_to_file(GapBuffer* gap_buffer, const char* filename, const char* file_extension) {
    char* target = strcat(filename, file_extension);
    FILE* file = fopen(target, "w");


    for (int i = 0; i < gap_buffer->size; i++) {
        if (i < gap_buffer->gap_start || i >= gap_buffer->gap_end) {
            fputc(gap_buffer->char_buffer[i], file);
        }
    }

    // 파일 닫기
    fclose(file);
}