OBJS = gap_buffer.o terminal.o viva.o
TARGET = viva.out

# 기본 타겟
all: $(TARGET)

# 최종 실행 파일 생성 규칙
$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS) -L./lib -lm

# 개별 오브젝트 파일 생성 규칙
gap_buffer.o: gap_buffer.h gap_buffer.c
	gcc -c -o gap_buffer.o gap_buffer.c -Wall -Wextra -std=c11 -I./include

terminal.o: terminal.h terminal.c
	gcc -c -o terminal.o terminal.c -Wall -Wextra -std=c11 -I./include

viva.o: viva.c
	gcc -c -o viva.o viva.c -Wall -Wextra -std=c11 -I./include

# OS에 따라 clean 규칙 설정
clean:
ifeq ($(OS),Windows_NT)
	del $(OBJS) $(TARGET)
else
	rm -f $(OBJS) $(TARGET)
endif

# PHONY 타겟 설정
.PHONY: all clean
