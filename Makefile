CC=g++
CFLAGS=-c -Wall -std=c++11

LDFLAGS=
INCLUDE=-I./utils

SOURCES=main.cpp ./utils/Logger.cpp
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=main

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)