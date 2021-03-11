# Makefile for the Salt compiler for Linux based system.

MAIN   = saltc.cpp
RESULT = saltc

CFLAGS = -std=c++1z

SOURCES = $(shell find src -name \*.cpp | sed 's/src\///' | tr '\n' ' ') 
OBJECTS = $(foreach var,$(SOURCES),build/objects/$(var).o)

# Execute all the steps
all: directories $(SOURCES) final

# Clean after compilation, moving the compiled result to
# the top folder.
clean:
	mv build/$(RESULT) ./$(RESULT)
	rm -r build

# Create all the needed directories.
directories:
	mkdir -p build/objects
	mkdir -p build/objects/arg_parser
	mkdir -p build/objects/tokenizer

# Compile all sources to object files
$(SOURCES):
	g++ -c $(CFLAGS) -o build/objects/$@.o src/$@

# Compile and link the main file
final:
	g++ $(CFLAGS) -o build/$(RESULT) $(MAIN) $(OBJECTS)
