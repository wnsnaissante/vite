# Variables
CC = gcc
CFLAGS = -Wall -Wextra -O2 -march=native -funroll-loops -fomit-frame-pointer -I./include -L./lib
OBJS = gap_buffer.o terminal.o viva.o
TARGET = viva.out

# Detect platform
ifeq ($(OS),Windows_NT)
    LDFLAGS = -L./lib -lpdcurses -lm
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LDFLAGS = -L./lib -lncurses -lm
    else ifeq ($(UNAME_S),Darwin)
        LDFLAGS = -L./lib -lncurses -lm
    endif
endif

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
