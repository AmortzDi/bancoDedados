CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
TARGET = fdtool
OBJS = main.o parser.o closure.o mincover.o keys.o normalform.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)