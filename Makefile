CFLAGS= -Wall -Wextra
CFLAGS+= -g
LDFLAGS= -lelf

all: ian-proj1

.PHONY: clean pack
clean:
	rm ian-proj1 ian-proj1.tar.gz

pack:
	tar --gz -vcf ian-proj1.tar.gz ian-proj1.c Makefile
