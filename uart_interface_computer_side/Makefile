CC = gcc
CFLAGS = -Wall
LDFLAGS = -lpthread -lserialport -lwolfssl -L.
OBJFILES = crypto.o main.o
TARGET = interface_uart.out

all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~