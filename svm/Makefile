# The Salt Programming Language Developers, 2021

# This is the core Makefile for building SVM for Linux, which provides
# diffrent kind of builds, for production, debug and also Windows builds
# using gcc-mingw. The main target is set to __all, which is also the
# first target which translates to the default target for make.

# Just use bash...

SHELL = /bin/bash


# Stop users from executing internal targets.

$(if $(filter __%, $(MAKECMDGOALS)), $(error "Cannot use internal targets"))


# The target variable is set to the target system, linux for 64 bit 
# Linux systems and win32 or win64 depending on the goal.

TARGET := linux
win32 : TARGET := win32
win64 : TARGET := win64


# The build version is defined when before compilation, pulling data from
# the version file first, adding a number to the build and then setting
# the version to it.

VERSIONFILE = "version.conf"
VERSION := 
TITLE_VERSION = $(shell echo "$(VERSION)" | tr -d '\\')


# These are the filenames for the main function file, and the output name. 

MAIN   = main.c
RESULT = svm


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

CC = clang


# The default location of Python 3 is in /usr/bin/python3, but we want to
# fetch the latest version of Python from the users' computer. This is done
# in the __fetch_python subroutine. 

PYTHON3 = /usr/bin/python3


# compilers instead. We don't support windows clang yet! (I don't even know
# if there is a windows clang for linux for cross compiling)

win32 : CC := i686-w64-mingw32-gcc
win64 : CC := x86_64-w64-mingw32-gcc


# Include path for the SVM headers.

INCLUDE_FLAGS = -I$(PWD)/include

# The default CFLAGS are all set to all warnings, and the newest C version,
# but we turn off unused parameters because it's just useless, and shows
# the unused parameters in exec_ functions even if they are supposed to 
# work like that.

CFLAGS = -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c2x \
	      -O3 -Wno-unused-result


# Here are flags which only work if clang is the compiler of choice.
# We want to enable a lot of warnings, but some are just useless.

CLANG_FLAGS = -Wdocumentation


# Remember to set different CFLAGS if we're compiling for debug mode.

debug : CFLAGS := -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c2x \
                  -Wno-unused-result -ggdb


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


# The help target will list all current possible targets. Use this when you
# don't know which target to use for your goal.

help:
	printf "\
	SVM build system. To use one of these targets, just use 'make TARGETNAME'\n\n\
	It is highly recommended to use clang instead of gcc, because of the additional\n\
	functionality & static checking it provides. This Makefile is built around \n\
	clang, but gcc is also supported\n\n\
	Targets:\n\n\
		[default] 	build the production version of SVM, for Linux\n\
		debug		build the debug version for Linux\n\
		win32		build for Windows 32-bit, requires mingw-gcc\n\
		win64 		build for Windows 64-bit, requires mingw-gcc\n\
		help		show this page and exit\n\
		install		install the built binaries in SALT_HOME/bin\n\
		clean		clean the build directory\n\
		test		runs the svm_test app written in Python\n\
	\n\
	Variables:\n\n\
		CC		the compiler used\n\
		MAIN		.c file with the main fucntion\n\
		RESULT 		name of the built result\n\
		MAKE_CFLAGS 	additional flags for compilation\n\
		MAKE_DEBUG	if this is set to 1, print debug information\n\
	\n"
	exit 0

.PHONY  : help
# Make help always silent, even if you specify the debug version.
.SILENT : help


# The clean target removes the build directory where all object files stay.

clean:
	@echo "Cleaning build directory"
	@rm -rf build
.PHONY : clean


# The installation target. This installs the compiled executable into the
# $SALT_HOME/bin directory. This requires $SALT_HOME to be defined.

install: 
	echo "Checking for built executable"
	if [ -x "build/$(RESULT)" ]; then \
		echo "Found executable"; \
		if [ -n "${SALT_HOME}" ]; then \
			echo "Found salt home, moving executable"; \
			mv build/$(RESULT) ${SALT_HOME}/bin/$(RESULT); \
		else \
			echo "Cannot find salt home."; \
		fi \
	fi

.PHONY : install
ifneq ($(MAKE_DEBUG), 1)
.SILENT : install
endif

# The test target only runs all tests and prints the results. 

test: __fetch_python
	@cd ./tests; $(PYTHON3) ./svm_test.py
.PHONY : test


# -----------------------------------------------------------------------------
# Internal subroutines
# -----------------------------------------------------------------------------

# The setup target is called before every compilation target. It is 
# responsible for creating directories and cleaning unwated old files.

__setup: __fetch_python
	$(eval VERSION := $(shell $(PYTHON3) build.py $(VERSIONFILE)))
	
	printf "Building Salt Virtual Machine $(TITLE_VERSION) for $(TARGET) [$(CC)]\n"
	
	if [ "${MAKE_CFLAGS}" != "" ]; then \
		echo "Using enviromental MAKE_CFLAGS = ${MAKE_CFLAGS}"; \
	fi

	echo "Creating directories"
	mkdir -p build
	mkdir -p build/objects

.PHONY  : __setup
ifneq ($(MAKE_DEBUG), 1)
.SILENT : __setup
endif


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
		printf "Build \& testing SVM requires Python \>= 3.9"; \
		exit 1; \
	else \
		printf "Found $(firstword $(T_PYTHON_3))\n"; \
	fi

	$(eval PYTHON3 := $(firstword $(T_PYTHON_3)))

.PHONY  : __fetch_python
ifneq ($(MAKE_DEBUG), 1)
.SILENT : __fetch_python
endif


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
	echo "Compiling $@"
	$(CC) $(CFLAGS) -D SVM_VERSION=\"$(VERSION)\" $(INCLUDE_FLAGS) $(DFLAGS) \
		${MAKE_CFLAGS} -c -o build/objects/$@.o src/$@

ifneq ($(MAKE_DEBUG), 1)
.SILENT : $(SOURCES)
endif


# The link target is the final step in compilation. Note: if the MAKE_CFLAGS
# enviroment variable is defined, it will be placed at the end of the linking
# instruction.

__link:
	echo "Linking objects with main"
	$(CC) $(CFLAGS) -D SVM_VERSION=\"$(VERSION)\" $(INCLUDE_FLAGS) $(DFLAGS) \
		-o build/$(RESULT) ${MAKE_CFLAGS} $(MAIN) $(OBJECTS)
	printf "\e[92mDone\e[0m\n"

ifneq ($(MAKE_DEBUG), 1)
.SILENT : __link
endif


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

