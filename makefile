default: main

.PHONY: main clean flags bindir

PKG_CONFIG_PATH="$(shell pwd)"
CFLAGS=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs --cflags scratch)
SOURCES=$(wildcard src/**/*.cpp src/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

main: bindir $(OBJECTS)
	$(CXX) -o bin/main -D_X_OPEN_SOURCE=700 $(OBJECTS) $(CFLAGS)

%.o: %.cpp
	$(CXX) -c -D_X_OPEN_SOURCE=700 $(CFLAGS) $< -o $@

flags:
	@echo $(CFLAGS)

bindir:
	mkdir -p bin

clean:
	rm -f bin/* $(OBJECTS)
