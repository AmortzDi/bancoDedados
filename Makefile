CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
TARGET = fdtool
SRCS = main.c types.c lerarquivo.c closure.c mincover.c keys.c normalform.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)