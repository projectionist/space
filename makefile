default: main

.PHONY: main clean bindir

CFLAGS=$(shell pkg-config wayland-client wayland-egl wayland-cursor egl glesv2 libdrm --cflags --libs)
SOURCES=$(wildcard src/**/*.cpp src/*.cpp)

main: bindir $(SOURCES)
	$(CXX) -std=c++14 -pthread -pedantic -Wall -Wextra -O3 -Isrc -D_X_OPEN_SOURCE=700 $(CFLAGS) $(SOURCES) -o bin/main

bindir:
	mkdir -p bin

clean:
	rm -f bin/*
