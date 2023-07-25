CC		= gcc
CFLAGS	= -pthread -ldiscord -lcurl
TARGET	= dlrb

all: $(TARGET)

$(TARGET):
	$(CC) $(TARGET).c -o $(TARGET) $(CFLAGS) -g

clean:
	-rm $(TARGET)

run: $(TARGET)
	./$(TARGET) config.json
