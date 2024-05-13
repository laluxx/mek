CC = clang
TARGET = mek
CFLAGS = -Wall -Wextra -I.
LFLAGS = 
SOURCES = ./main.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LFLAGS) -o $(TARGET)

.PHONEY: clean
clean:
	@rm -f $(TARGET)

.PHONEY: remove
remove: clean
	@rm -f $(TARGET)
