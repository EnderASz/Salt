# Makefile for the Salt Virtual Machine for Linux based system.

MAIN   = svm.c
RESULT = svm

C_SOURCES = $(shell find src -name \*.c | sed 's/src\///' | tr '\n' ' ') 
OBJECTS = $(foreach var,$(C_SOURCES),build/objects/$(var).o) 

# Execute all the steps
all: directories objects final

# Clean after compilation, moving the compiled result to
# the top folder.
clean:
	mv build/$(RESULT) ./$(RESULT)
	rm -r build

# Create all the needed directories.
directories:
	mkdir -p build
	mkdir -p build/objects

# Compile all sources to object files
objects:
	$(foreach var,$(C_SOURCES),gcc -c -o build/objects/$(var).o src/$(var);)

# Compile and link the main file
final:
	gcc -o build/$(RESULT) $(MAIN) $(OBJECTS)

# Clean and move the compiled file to the user's ~/bin
install: clean
	mkdir -p ~/bin
	mv $(RESULT) ~/bin/$(RESULT)
