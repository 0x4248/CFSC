CC = gcc
CFLAGS = -pthread

SRC = src
MAIN = main.c

OUTPUT = cfsc

all: $(OUTPUT)

$(OUTPUT): $(SRC)/$(MAIN)
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm -f $(OUTPUT)

install:
	cp $(OUTPUT) /usr/bin/$(OUTPUT)

uninstall:
	rm -f /usr/bin/$(OUTPUT)