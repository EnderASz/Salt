# Makefile for the Salt Virtual Machine for Linux based system.

MAIN   = svm.c
RESULT = svm

SOURCES = $(shell find src -name \*.c | sed 's/src\///' | tr '\n' ' ') 
OBJECTS = $(foreach var,$(SOURCES),build/objects/$(var).o) 

OPT = -O

# Execute all the steps
all: directories $(SOURCES) final

# Clean after compilation, moving the compiled result to
# the top folder.
clean:
	mv build/$(RESULT) ./$(RESULT)
	rm -r build

# Create all the needed directories.
directories:
	# Creating directories
	mkdir -p build/objects

# Compile all sources to object files
$(SOURCES):
	gcc -c -o build/objects/$@.o src/$@

# Compile and link the main file
final:
	# Final compilation step
	gcc $(OPT) -o build/$(RESULT) $(MAIN) $(OBJECTS)
	
	# ----- Done ----- 	
