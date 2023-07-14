CC = gcc
CFLAGS = -Wall -std=c99 -Wextra
INCLUDE_PATHS = -I ./include/
LDFLAGS = -lraylib -lm
SOURCES = src/*.c
TARGET = cgame

.PHONY: all

all: $(TARGET)

$(TARGET): build

build:
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE_PATHS) $(SOURCES) -o $(TARGET)

run: build
	./$(TARGET)

clean:
	@rm $(TARGET)
