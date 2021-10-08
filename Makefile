TARGET=wmsync

IDIR = include
DIRS = * src/wmsync/* $(IDIR)/inih/* $(IDIR)/log.c/*

CC = gcc
CFLAGS = -I $(IDIR) -Wall
LIBS = -liphlpapi -lws2_32

HEADERS = $(foreach dir, $(DIRS), $(wildcard $(dir).h))
OBJECTS = $(patsubst %.c, %.o, $(foreach dir, $(DIRS), $(wildcard $(dir).c)))

.PHONY: default all clean

default: $(TARGET)
all: default clean

src/wmsync/%.o: src/wmsync/%.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS)