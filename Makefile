OBJS = gap_buffer.o terminal.o viva.o
TARGET = viva.out

# �⺻ Ÿ��
all: $(TARGET)

# ���� ���� ���� ���� ��Ģ
$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS) -L./lib -lm

# ���� ������Ʈ ���� ���� ��Ģ
gap_buffer.o: gap_buffer.h gap_buffer.c
	gcc -c -o gap_buffer.o gap_buffer.c -Wall -Wextra -std=c11 -I./include

terminal.o: terminal.h terminal.c
	gcc -c -o terminal.o terminal.c -Wall -Wextra -std=c11 -I./include

viva.o: viva.c
	gcc -c -o viva.o viva.c -Wall -Wextra -std=c11 -I./include

# OS�� ���� clean ��Ģ ����
clean:
ifeq ($(OS),Windows_NT)
	del $(OBJS) $(TARGET)
else
	rm -f $(OBJS) $(TARGET)
endif

# PHONY Ÿ�� ����
.PHONY: all clean
