# The Salt Programming Language Developers, 2021

# This is the core Makefile for building SVM for Linux, which provides
# diffrent kind of builds, for production, debug and also Windows builds
# using gcc-mingw. The main target is set to __all, which is also the
# first target which translates to the default target for make.

# Just use bash...

SHELL=/bin/bash


# Stop users from executing internal targets.

$(if $(filter __%, $(MAKECMDGOALS)), $(error "Cannot use internal targets"))


# The target variable is set to the target system, linux64 for 64 bit 
# Linux systems and win32 or win64 depending on the goal.

TARGET := linux64
win32 : TARGET := win32
win64 : TARGET := win64


# The build version is defined when before compilation, pulling data from
# the version file first, adding a number to the build and then setting
# the version to it.

VERSIONFILE := "version.conf"
VERSION := 
TITLE_VERSION := $(shell echo "$(VERSION)" | tr -d '\\')


# These are the filenames for the main function file, and the output name. 

MAIN   := svm.c
RESULT := svm


# Root folder of the makefile. Used in operations which require directory
# changing. a cd to $(ROOT) will always return you to the initial area, 
# where the Makefile is located.

ROOT = $(abspath .)


# This makefile supports both gcc and clang, so probably 99% of the used
# compilers (in the Linux world, I know there is msvc). It's set to use
# clang by default, because it has a lot of cool stuff like sanitizers
# that really help in debug builds of svm. If you don't have clang and 
# just want to compile using gcc, pass it as a parameter in the make 
# command, like so: `make CC=gcc`.

CC := clang


# The default location of Python 3 is in /usr/bin/python3, but we want to
# fetch the latest version of Python from the users' computer. This is done
# in the __fetch_python subroutine. 

PYTHON3 := /usr/bin/python3


# compilers instead. We don't support windows clang yet! (I don't even know
# if there is a windows clang for linux for cross compiling)

win32 : CC := i686-w64-mingw32-gcc
win64 : CC := x86_64-w64-mingw32-gcc


# The default CFLAGS are all set to all warnings, and the newest C version,
# but we turn off unused parameters because it's just useless, and shows
# the unused parameters in exec_ functions even if they are supposed to 
# work like that.

CFLAGS = -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c2x \
	      -fstack-protector-strong -O3 -Wno-unused-result


# Here are flags which only work if clang is the compiler of choice.
# We want to enable a lot of warnings, but some are just useless.

CLANG_FLAGS = -Wdocumentation


# Remember to set different CFLAGS if we're compiling for debug mode.

debug : CFLAGS := -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c2x \
                  -fstack-protector-strong -Wno-unused-result -ggdb


# Options for clang only.

ifeq ($(CC), clang)
debug : DFLAGS += -Wunused-command-line-argument
endif


# DEBUG is defined it the debug target is selected. Additionally, if the 
# compiler is clang, the sanitizer parameters are strapped onto the executable.

debug : DFLAGS := -D DEBUG -pg -O1
debug : RESULT := svm

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
# get on with your life. Imporant note: this target has to be the first target,
# because it is the default one.

__all: __setup $(SOURCES) __link
.PHONY : __all


# The clean target removes the build directory where all object files stay.

clean:
	@echo "Cleaning build directory"
	@rm -rf build
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


# The test target only runs all tests and prints the results. 

test: __fetch_python
	@cd ./tests; $(PYTHON3) ./svm_test.py
.PHONY : test


# -----------------------------------------------------------------------------
# Internal subroutines
# -----------------------------------------------------------------------------

# The setup target is called before every compilation target. It is 
# responsible for creating directories and cleaning unwated old files.

__setup:
	@printf "Building Salt Virtual Machine $(TITLE_VERSION) for $(TARGET) [$(CC)]\n"
	
	$(eval VERSION := $(shell python3.9 build.py $(VERSIONFILE)))

	@if [ "${MAKE_CFLAGS}" != "" ]; then \
		echo "Using enviromental MAKE_CFLAGS = ${MAKE_CFLAGS}"; \
	fi

	@echo "Creating directories"
	@mkdir -p build
	@mkdir -p build/objects
.PHONY : __setup


# This subroutine is responsible for finding the newest version of Python 3 
# which is install on the user's computer.

__fetch_python:
	printf "Looking for newest Python 3 version\n"

	$(eval T_ARRAY := $(shell echo $(PATH) | tr ':' '\n'))

	$(eval T_RESULT := $(shell for i in $(T_ARRAY); do \
		ls $${i}; \
	done))

	$(eval T_PYTHON_3 := $(findstring python3.10, $(T_RESULT)) $(findstring python3.9, $(T_RESULT))) 

	if [ "$(findstring python, $(T_PYTHON_3))" != "python" ]; then \
		printf "The python version you own is too old for building SVM\n"; \
		exit 1; \
	else \
		printf "Found $(firstword $(T_PYTHON_3))\n"; \
	fi

	$(eval PYTHON3 := $(firstword $(T_PYTHON_3)))

.PHONY  : __fetch_python
.SILENT : __fetch_python


# The compile target is called compiling the source files into object files.
# All objects are saved in build/objects, which then are linked in __link.
# Note: if the MAKE_CFLAGS enviroment variable is defined, it will be placed
# at the end of the compilation instruction.
# We're using a simple bash evaluation here to determine if we should put 
# clang analyzer flags, because make if statements just don't seem to work 
# as I expect them to. Too bad.

$(SOURCES):
	$(eval CFLAGS := $(shell \
		if [ "$(CC)" == "clang" ]; then \
			echo "$(CFLAGS) $(CLANG_FLAGS)"; \
		else \
			echo "$(CFLAGS)"; \
		fi \
	))
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -D SVM_VERSION=\"$(VERSION)\" $(DFLAGS) $(UFLAGS) ${MAKE_CFLAGS} \
		-c -o build/objects/$@.o src/$@


# The link target is the final step in compilation. Note: if the MAKE_CFLAGS
# enviroment variable is defined, it will be placed at the end of the linking
# instruction.

__link:
	@echo "Linking objects with main"
	@$(CC) $(CFLAGS) -D SVM_VERSION=\"$(VERSION)\" $(DFLAGS) -o build/$(RESULT) ${MAKE_CFLAGS} \
		$(MAIN) $(OBJECTS)
	@printf "\e[92mDone\e[0m\n"


# -----------------------------------------------------------------------------
# Non-default targets
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

