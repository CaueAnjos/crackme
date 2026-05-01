INCLUDE = -Iinclude -Isrc

LDFLAGS = -Llib
LIBS = -lMinHook

PKGCONFIG ?= pkg-config

all: lua injector injected

lua:
	mkdir -p bin
	$(CC) src/*.c -o bin/crackme $(INCLUDE)  $(shell $(PKGCONFIG) --cflags --libs lua)

injector: 
	mkdir -p bin
	$(CXX) src/injector/*.cpp $(LIBS) -static -o bin/injector -mwindows $(INCLUDE) $(LDFLAGS)

injected:
	mkdir -p bin/lib
	$(CXX) src/injectedDll/*.cpp $(LIBS) -static -shared -o bin/lib/hook.dll -mwindows $(INCLUDE) $(LDFLAGS)

clean:
	rm -rf bin/
