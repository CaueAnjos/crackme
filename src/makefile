INCLUDE = -Iinclude
SRC = src/*.c
OUT = bin/crackme

PKGCONFIG ?= pkg-config

all:
	mkdir -p "bin"
	$(CC) $(SRC) -o $(OUT) $(INCLUDE) $(shell $(PKGCONFIG) --cflags --libs lua)
