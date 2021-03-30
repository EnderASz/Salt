# Makefile for the Salt Virtual Machine for Linux based system.

MAIN   = svm.c
RESULT = svm

SOURCES = $(shell find src -name \*.c | sed 's/src\///' | tr '\n' ' ') 
OBJECTS = $(foreach var,$(SOURCES),build/objects/$(var).o) 

CC := clang
CFLAGS := -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c2x 

# Clang options

VERSION := -D SVM_VERSION=\"$(shell python3 build.py build)\"

debug : RESULT := svmd
debug : FDEBUG := -D DEBUG=1
#debug : CLANG_FLAGS := -fsanitize=memory -fno-omit-frame-pointer \
#	    -fno-optimize-sibling-calls -O1
debug : CLANG_FLAGS := -fsanitize=address -fno-omit-frame-pointer \
		-fno-optimize-sibling-calls -O1
win32 : CC := i686-w64-mingw32-gcc
win32 : RESULT := svm32
win64 : CC := x86_64-w64-mingw32-gcc
win64 : RESULT := svm64

# Execute all the steps
all: directories $(SOURCES) link
.PHONY: all

# Target for cross-compiling onto 32 bit Windows
win32: all 
.PHONY: win32

# Target for cross-compiling onto 64 bit windows
win64: all
.PHONY: win64

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
	$(CC) $(CFLAGS) $(CLANG_FLAGS) $(VERSION) $(FDEBUG) -c -o build/objects/$@.o src/$@

# Compile and link the main file
link:
	$(CC) $(VERSION) $(CLANG_FLAGS) $(FDEBUG) -o build/$(RESULT) $(MAIN) $(OBJECTS)
.PHONY: link
