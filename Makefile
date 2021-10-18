TARGET=wmsync

IDIR = include
DIRS = * src/wmsync/* $(IDIR)/inih/* $(IDIR)/log.c/* $(IDIR)/libhttp/* $(IDIR)/wmsync/*

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wconversion -I $(IDIR) -DLOG_USE_COLOR
LIBS = -liphlpapi -lws2_32

HEADERS = $(foreach dir, $(DIRS), $(wildcard $(dir).h))
OBJECTS = $(patsubst %.c, %.o, $(foreach dir, $(DIRS), $(wildcard $(dir).c)))

.PHONY: default all clean

default: all
all: $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS)