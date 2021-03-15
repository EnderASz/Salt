# Makefile for the Salt Virtual Machine for Linux based system.

MAIN   = svm.c
RESULT = svm

SOURCES = $(shell find src -name \*.c | sed 's/src\///' | tr '\n' ' ') 
OBJECTS = $(foreach var,$(SOURCES),build/objects/$(var).o) 

CFLAGS = -Wall

# Execute all the steps
all: directories version $(SOURCES) link
.PHONY: all

# Add version control
version:
	cp include/os.h build/os.h.clean
	python3 build.py build > include/os.h
	cat build/os.h.clean >> include/os.h
.PHONY: version

# Build a debug mode
debug: directories version debug-setup $(SOURCES) link debug-clean
.PHONY: debug

# Setup the debug define in utils  
debug-setup: 
	cp include/utils.h build/utils.h.clean
	printf "#define DEBUG\n" > include/utils.h
	cat build/utils.h.clean >> include/utils.h
.PHONY: debug-setup

# Clean after the debug
debug-clean:
	mv build/utils.h.clean include/utils.h
	# Built debug version
.PHONY: debug-clean

# Clean after compilation, moving the compiled result to
# the top folder.
clean:
	mv build/$(RESULT) ./$(RESULT)
	rm -r build
.PHONY: clean

# Create all the needed directories.
directories:
	# Creating directories
	mkdir -p build/objects
.PHONY: directories

# Compile all sources to object files
$(SOURCES):
	gcc $(CFLAGS) -c -o build/objects/$@.o src/$@

# Compile and link the main file
link:
	gcc -o build/$(RESULT) $(MAIN) $(OBJECTS)
	mv build/os.h.clean include/os.h
.PHONY: link
