# The Salt Programming Language Developers, 2021

# The core Makefile for Linux systems for building the Salt Compiler.
# The only target for this Makefile is 64-bit Linux.

# -----------------------------------------------------------------------------
# Global settings
# -----------------------------------------------------------------------------

# Filenames for the main function file & the result

MAIN   := saltc.cpp
RESULT := saltc

# C++ Compilation settings

CXXC := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wno-unknown-pragmas

# Source files

SOURCES = $(shell find src -name \*.cpp | sed s'/src\///' | tr '\n' ' ') 
OBJECTS = $(foreach var,$(SOURCES),build/objects/$(var).o)

# -----------------------------------------------------------------------------
# Targets
# -----------------------------------------------------------------------------

.PHONY: all
# Execute all the steps
all: directories
	make build -j
	make final


.PHONY: clean
# Clean after compilation, moving the compiled result to
# the top folder.
clean:
	@if [ -x 'build/$(RESULT)' ]; then \
		mv -f build/$(RESULT) ./$(RESULT); \
	fi
	rm -rf build


.PHONY: directories
# Create all the needed directories.
directories:
	mkdir -p build/objects
	mkdir -p build/objects/tokenizer
	mkdir -p build/objects/scc


.PHONY: build
# This is a seperate target to build the object files on multiple
# processes. 
build: $(SOURCES)

# Compile all sources to object files
$(SOURCES):
	$(CXXC) -c $(CXXFLAGS) -D SALT_DEBUG -D __FILENAME__=\"$@\" -o build/objects/$@.o src/$@

.PHONY: final
# Compile and link the main file
final:
	$(CXXC) $(CXXFLAGS) -D SALT_DEBUG -D __FILENAME__=\"$(MAIN)\" -o build/$(RESULT) $(MAIN) $(OBJECTS)

