# Projection makefile
#
# Behaviours
#
# * Compiler and linker flags collected with pkg-config
# * source files (.c, .cpp) precompiled to object files (.o)
# * object files are rebuilt when source or dependencies change
#
# Reading material for makefiles
#
# * http://www.gnu.org/software/make/manual/
# * http://people.freedesktop.org/~dbn/pkg-config-guide.html
# * http://make.mad-scientist.net/papers/rules-of-makefiles/
# * http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/#tldr
# * http://eigenstate.org/notes/makefiles
#

PKG_CONFIG_PATH="$(shell pwd)"
PKG_CONFIG_CMD=PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs --cflags space
CFLAGS=$(shell $(PKG_CONFIG_CMD))
SOURCES=$(wildcard src/**/*.cpp src/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))
BINDIR=bin
MAIN=bin/main

# automatically rebuilding when a source or any of its dependencies change
DEPFLAGS=-MT $@ -MD -MP -MF $*.Td
DEPFILES=$(wildcard src/**/*.d src/*.d) $(wildcard src/**/*.Td src/*.Td)
POSTCOMPILE=mv -f $*.Td $*.d

default: $(MAIN)
.PHONY: clean $(BINDIR) pkg-config
.SUFFIXES:
.SECONDARY:

$(MAIN): $(OBJECTS) | $(BINDIR) pkg-config
	$(CXX) -o $(MAIN) $(OBJECTS) $(CFLAGS)

%.o: %.cpp %.d | pkg-config
	$(CXX) -c $(DEPFLAGS) $(CFLAGS) $< -o $@
	$(POSTCOMPILE)

%.d: ;

$(BINDIR):
	@mkdir -p $(BINDIR)

# check the pkg-config command returns successfully
pkg-config:
	$(PKG_CONFIG_CMD)

clean:
	rm -f $(BINDIR)/* $(OBJECTS) $(DEPFILES)

-include $(patsubst %,%.d,$(basename $(SOURCES)))
