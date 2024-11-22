# Variables
CC = gcc
CFLAGS = -Wall -Wextra -O2 -march=native -funroll-loops -fomit-frame-pointer -I./include -L./lib
LDFLAGS = -L./lib -lpdcurses -lm
OBJS = gap_buffer.o terminal.o viva.o
TARGET = viva.out

# Targets
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

viva.o: viva.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
ifeq ($(OS),Windows_NT)
	del $(OBJS) $(TARGET)
else
	rm -f $(OBJS) $(TARGET)
endif

.PHONY: all clean
