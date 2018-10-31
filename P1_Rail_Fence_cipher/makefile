TRGT = main
LIBS = -lm
CFLAGS = -g -Wall
CC = gcc
.PHONY: default all clean
default: $(TRGT)
all: default
HS = $(wildcard *.h)
OBJS = $(patsubst %.c, %.o, $(wildcard *.c))

%.o: %.c $(HS)
	$(CC) $(CFLAGS) -c $< -o $@
.PRECIOUS: $(TRGT) $(OBJS)
$(TRGT): $(OBJS)
	$(CC) $(OBJS) -Wall $(LIBS) -o $@
