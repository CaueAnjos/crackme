INCLUDE = -Iinclude
SRC = src/*.c
OUT = bin/crackme

all:
	mkdir -p "bin"
	$(CC) $(SRC) -o $(OUT) $(INCLUDE) $(shell pkg-config --cflags --libs lua)
