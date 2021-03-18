# Makefile for the Salt compiler for Linux based system.

MAIN   = saltc.cpp
RESULT = saltc

CFLAGS = -std=c++1z -Wall

SOURCES = $(shell find src -name \*.cpp | sed s'/src\///' | tr '\n' ' ') 
OBJECTS = $(foreach var,$(SOURCES),build/objects/$(var).o)

.PHONY: all
# Execute all the steps
all: directories
	make build -j
	make final


.PHONY: clean
# Clean after compilation, moving the compiled result to
# the top folder.
clean:
	mv build/$(RESULT) ./$(RESULT)
	rm -r build


.PHONY: directories
# Create all the needed directories.
directories:
	mkdir -p build/objects
	mkdir -p build/objects/arg_parser
	mkdir -p build/objects/tokenizer
	mkdir -p build/objects/scc
	mkdir -p build/objects/errors

.PHONY: build
# This is a seperate target to build the object files on multiple
# processes. 
build: $(SOURCES)

# Compile all sources to object files
$(SOURCES):
	g++ -c $(CFLAGS) -o build/objects/$@.o src/$@


.PHONY: final
# Compile and link the main file
final:
	g++ $(CFLAGS) -o build/$(RESULT) $(MAIN) $(OBJECTS)
