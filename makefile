# Projection makefile
#
# Behaviours
#
# * Compiler and linker flags collected with pkg-config
# * source files (.c, .cpp) precompiled to object files (.o)
# * sources are rebuilt when header dependencies change
# * headers are precompiled and recompiled automatically
#
#
# Reading material for makefiles
#
# * http://www.gnu.org/software/make/manual/
# * http://people.freedesktop.org/~dbn/pkg-config-guide.html
# * http://make.mad-scientist.net/papers/rules-of-makefiles/
# * http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/#tldr
#

PKG_CONFIG_PATH="$(shell pwd)"
CFLAGS=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs --cflags scratch)
SOURCES=$(wildcard src/**/*.cpp src/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))
BINDIR=bin
MAIN=bin/main

default: $(MAIN)
.PHONY: clean $(BINDIR)

$(MAIN): $(OBJECTS) | $(BINDIR)
	$(CXX) -o $(MAIN) $(OBJECTS) $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -f $(BINDIR)/* $(OBJECTS) $(PRECOMPILED_HEADERS)
