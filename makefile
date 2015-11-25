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
#

PKG_CONFIG_PATH="$(shell pwd)"
CFLAGS=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs --cflags scratch)
SOURCES=$(wildcard src/**/*.cpp src/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))
BINDIR=bin
MAIN=bin/main

# automatically rebuilding when a source or any of its dependencies change
DEPFLAGS=-MT $@ -MMD -MP -MF $*.Td
DEPFILES=$(wildcard src/**/*.d src/*.d) $(wildcard src/**/*.Td src/*.Td)
POSTCOMPILE=mv -f $*.Td $*.d

default: $(MAIN)
.PHONY: clean $(BINDIR)

$(MAIN): $(OBJECTS) | $(BINDIR)
	$(CXX) -o $(MAIN) $(OBJECTS) $(CFLAGS)

%.o: %.cpp %.d
	$(CXX) -c $(DEPFLAGS) $(CFLAGS) $< -o $@
	$(POSTCOMPILE)

%.d: ;

$(BINDIR):
	@mkdir -p $(BINDIR)

clean:
	rm -f $(BINDIR)/* $(OBJECTS) $(DEPFILES)

-include $(patsubst %,%.d,$(basename $(SOURCES)))
