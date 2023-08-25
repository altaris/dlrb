CC		= g++
CFLAGS	= -pthread -ldiscord -lcurl
TARGET	= dlrb

all: $(TARGET)

$(TARGET):
	$(CC) $(TARGET).cpp -o $(TARGET).out $(CFLAGS)

clean:
	-rm $(TARGET).out

run: $(TARGET)
	./$(TARGET).out config.json
