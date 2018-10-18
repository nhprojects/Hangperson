.PHONY: clean

CC=clang
CFLAGS=-std=c99 -Wall -g 
OBJS=hangperson.o 
TARGET=hangperson

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) 

clean:
	$(RM) $(OBJS) $(TARGET)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)
