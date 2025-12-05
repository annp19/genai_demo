CC = gcc
CFLAGS = -Wall -Wextra -O2 `pkg-config --cflags gtk+-3.0` `pkg-config --cflags cairo` -Iinclude
LDFLAGS = `pkg-config --libs gtk+-3.0` `pkg-config --libs cairo` -lmosquitto -lcjson -lpthread

SRCS = src/main.c src/mqtt.c src/json_parser.c src/data.c src/simulator.c src/gui.c src/telemetry.c
OBJS = $(SRCS:.c=.o)

TARGET = telemetry_dashboard

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean