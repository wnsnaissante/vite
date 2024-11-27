CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native -funroll-loops -fomit-frame-pointer
OBJS = gap_buffer.o terminal.o viva.o
TARGET = viva

# Detect platform
ifeq ($(OS),Windows_NT)
    LDFLAGS = -L./lib -lpdcurses -lm
    INCLUDES = -I./include -L./lib
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LDFLAGS = -lncurses -lm
        INCLUDES =
    else ifeq ($(UNAME_S),Darwin)
        LDFLAGS = -lncurses -lm
        INCLUDES =
    endif
endif

# Targets
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)

viva.o: viva.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)

clean:
ifeq ($(OS),Windows_NT)
	del $(OBJS) $(TARGET)
else
	rm -f $(OBJS) $(TARGET)
endif

.PHONY: all clean
