# Makefile for the Salt Virtual Machine for Linux based system.

MAIN   = svm.c
RESULT = svm

SOURCES = $(shell find src -name \*.c | sed 's/src\///' | tr '\n' ' ') 
OBJECTS = $(foreach var,$(SOURCES),build/objects/$(var).o) 

CFLAGS := -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c2x
VERSION := -D SVM_VERSION=\"$(shell python3 build.py build)\"
debug : FDEBUG := -D DEBUG=1

# Execute all the steps
all: directories $(SOURCES) link
.PHONY: all

# Just a compilation target so you can use -j with make
compile: $(SOURCES)
.PHONY: compile

# Compile debug version
debug: all
.PHONY: debug

# Clean after compilation, moving the compiled result to
# the top folder.
clean:
	rm -r build
.PHONY: clean

# Create all the needed directories.
directories:
	# Creating directories
	mkdir -p build/objects
.PHONY: directories

# Compile all sources to object files
$(SOURCES):
	gcc $(CFLAGS) $(VERSION) $(FDEBUG) -c -o build/objects/$@.o src/$@

# Compile and link the main file
link:
	gcc $(VERSION) $(FDEBUG) -o build/$(RESULT) $(MAIN) $(OBJECTS)
.PHONY: link
