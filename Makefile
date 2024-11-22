OBJS = gap_buffer.o terminal.o viva.o
TARGET = viva.out

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS) -L./lib -lpdcurses -lm

gap_buffer.o: gap_buffer.h gap_buffer.c
	gcc -c -o gap_buffer.o gap_buffer.c -Wall -Wextra -I./include -L./lib

terminal.o: terminal.h terminal.c
	gcc -c -o terminal.o terminal.c -Wall -Wextra -I./include -L./lib

viva.o: viva.c
	gcc -c -o viva.o viva.c -Wall -Wextra -I./include -L./lib

clean:
ifeq ($(OS),Windows_NT)
	del $(OBJS) $(TARGET)
else
	rm -f $(OBJS) $(TARGET)
endif

.PHONY: all clean
