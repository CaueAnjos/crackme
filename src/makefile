INCLUDE = -Iinclude -Isrc

LDFLAGS = -Llib
LIBS = -lMinHook

PKGCONFIG ?= pkg-config

all: lua injector

lua:
	mkdir -p bin
	$(CC) src/*.c -o bin/crackme $(INCLUDE)  $(shell $(PKGCONFIG) --cflags --libs lua)

injector: 
	mkdir -p bin
	$(CXX) src/*.cpp $(LIBS) -static -o bin/injector -mwindows $(INCLUDE) $(LDFLAGS)

clean:
	rm -rf bin/
	rm -f crackme.exe injector.exe
