INCLUDE = -Iinclude -Isrc
SRC = src/*.c
TARGET = crackme

LDFLAGS = -Llib
LIBS = lib/libMinHook.x64.lib

PKGCONFIG ?= pkg-config

all:
	mkdir -p "bin"
	$(CC) $(SRC) $(LIBS) -static -o $(TARGET) $(INCLUDE) $(LDFLAGS) $(shell $(PKGCONFIG) --cflags --libs lua)
