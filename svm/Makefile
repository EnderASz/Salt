# Makefile for the Salt Virtual Machine for Linux based system.

MAIN   = svm.c
RESULT = svm

SOURCES = $(shell find src -name \*.c | sed 's/src\///' | tr '\n' ' ') 
OBJECTS = $(foreach var,$(SOURCES),build/objects/$(var).o) 

OPT = -O

# Execute all the steps
all: directories version $(SOURCES) final
	# --- Done ---
.PHONY: all

# Add version control
version:
	cp include/os.h build/os.h.clean
	python3 build.py build > include/os.h
	cat build/os.h.clean >> include/os.h
.PHONY: version

# Build a debug mode
debug: directories debug-setup $(SOURCES) final debug-clean
	# --- Done ---
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
	gcc -c -o build/objects/$@.o src/$@

# Compile and link the main file
final:
	gcc $(OPT) -o build/$(RESULT) $(MAIN) $(OBJECTS)
	mv build/os.h.clean include/os.h
	# Built production version

.PHONY: final