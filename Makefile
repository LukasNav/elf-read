CFLAGS= -Wall -Wextra --std=c99
CFLAGS+= -g
LDFLAGS= -lelf

all: elf-read

.PHONY: clean pack
clean:
	rm elf-read

