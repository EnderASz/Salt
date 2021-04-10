# The Salt Programming Language Developers, 2021

# This is the core Makefile for building SVM for Linux, which provides
# diffrent kind of builds, for production, debug and also Windows builds
# using gcc-mingw. The main target is set to __all, which is also the
# first target which translates to the default target for make.

# The target variable is set to the target system, linux64 for 64 bit 
# Linux systems and win32 or win64 depending on the goal.

TARGET := linux64
win32 : TARGET := win32
win64 : TARGET := win64


# The build version is defined when before compilation, pulling data from
# the version file first, adding a number to the build and then setting
# the version to it.

VERSIONFILE := "version.conf"
VERSION := $(shell python3.9 build.py $(VERSIONFILE))
TITLE_VERSION := $(shell echo "$(VERSION)" | tr -d '\\')


# These are the filenames for the main function file, and the output name. 

MAIN   := svm.c
RESULT := svm


# This makefile supports both gcc and clang, so probably 99% of the used
# compilers (in the Linux world, I know there is msvc). It's set to use
# clang by default, because it has a lot of cool stuff like sanitizers
# that really help in debug builds of svm. If you don't have clang and 
# just want to compile using gcc, pass it as a parameter in the make 
# command, like so: `make CC=gcc`.

CC := clang


# The default CFLAGS are all set to all warnings, and the newest C version,
# but we turn off unused parameters because it's just useless, and shows
# the unused parameters in exec_ functions even if they are supposed to 
# work like that.

CFLAGS := -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -std=c2x \
		  -fstack-protector-strong -O3


# Remember to set different CFLAGS if we're compiling for debug mode.

debug : CFLAGS := -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c2x \
                  -fstack-protector-strong -ggdb


# Options for the clang static analyzer.

ifeq ($(CC), clang)
debug : DFLAGS += -Wunused-command-line-argument
endif

# If the targets are win32 or win64, this automatically chooses the gcc-mingw 
# compilers instead. We don't support windows clang yet! (I don't even know
# if there is a windows clang for linux for cross compiling)

win32 : CC := i686-w64-mingw32-gcc
win64 : CC := x86_64-w64-mingw32-gcc


# DEBUG is defined it the debug target is selected. Additionally, if the 
# compiler is clang, the sanitizer parameters are strapped onto the executable.

debug : DFLAGS := -D DEBUG -pg -O1
debug : RESULT := svmd

ifeq ($(CC), clang)
debug : DFLAGS += -fsanitize=address -fno-omit-frame-pointer \
		          -fno-optimize-sibling-calls
endif


# These are the sources and object paths which are just fetched using some 
# good old bash commands. The src/ prefix is removed from the path to only
# get the code.c path, and then the newlines are removed to move it into
# one line. The objects are also listed here, adding build/objects and 
# and a .o suffix.

SOURCES := $(shell find src -name \*.c | sed 's/src\///' | tr '\n' ' ')
OBJECTS := $(foreach var,$(SOURCES),build/objects/$(var).o)


# -----------------------------------------------------------------------------
# Internal targets
# -----------------------------------------------------------------------------

# The default target. This compiles and links the objects for a general
# Linux 64 bit executable. Use this target if you just want to build and 
# get on with your life.

__all: __setup $(SOURCES) __link
	@echo "Built standard"
.PHONY : __all


# The clean target removes the build directory where all object files stay.

clean:
	@echo "Cleaning build directory"
	rm -rf build
.PHONY : clean


# The installation target. This installs the compiled executable into the
# $SALT_HOME/bin directory. This requires $SALT_HOME to be defined.

install: 
	@echo "Checking for built executable"
	@if [ -x "build/$(RESULT)" ]; then \
		echo "Found executable"; \
		if [ -n "${SALT_HOME}" ]; then \
			echo "Found salt home, moving executable"; \
			mv build/$(RESULT) ${SALT_HOME}/bin/$(RESULT); \
		else \
			echo "Cannot find salt home."; \
		fi \
	fi

.PHONY : install


# The setup target is called before every compilation target. It is 
# responsible for creating directories and cleaning unwated old files.

__setup:
	@printf "Building Salt Virtual Machine $(TITLE_VERSION) for $(TARGET) [$(CC)]\n"
	
	@echo "Creating directories"
	mkdir -p build
	mkdir -p build/objects
.PHONY : __setup


# The compile target is called compiling the source files into object files.
# All objects are saved in build/objects, which then are linked in __link.
# Note: if the MAKE_CFLAGS enviroment variable is defined, it will be placed
# at the end of the compilation instruction.

$(SOURCES):
	$(CC) $(CFLAGS) -D SVM_VERSION=\"$(VERSION)\" $(DFLAGS) $(UFLAGS) ${MAKE_CFLAGS} \
		-c -o build/objects/$@.o src/$@


# The link target is the final step in compilation. Note: if the MAKE_CFLAGS
# enviroment variable is defined, it will be placed at the end of the linking
# instruction.

__link:
	$(CC) $(CFLAGS) -D SVM_VERSION=\"$(VERSION)\" $(DFLAGS) -o build/$(RESULT) ${MAKE_CFLAGS} \
		$(MAIN) $(OBJECTS)
	@printf "\e[92mDone\e[0m\n"


# -----------------------------------------------------------------------------
# Callable targets
# -----------------------------------------------------------------------------

# The debug target is used, well, for debugging purposes. The way this works is
# it compiles the program like usual, but with DEBUG defined, so all #ifdef DEBUG
# headers evaluate to true.

debug: __setup $(SOURCES) __link
	@echo "Built debug"
.PHONY : debug

# The windows scripts are similar, they just use diffrent compilers.

win32: __setup $(SOURCES) __link
	@echo "Built for Windows 32 bit"
.PHONY : win32

win64: __setup $(SOURCES) __link
	@echo "Built for Windows 64 bit"
.PHONY : win64

