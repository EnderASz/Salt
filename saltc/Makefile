# Copyright (C) throwbly, bellrise 2021


# Forbid the user from using internal targets

$(if $(filter _%, $(MAKECMDGOALS)), $(error "Cannot use internal targets"))


MAIN   := saltc.cpp
RESULT := saltc
CC     := g++
CFLAGS := -std=c++20 -Wall -Wextra -Wno-unknown-pragmas

SOURCES = $(shell find saltc/src -name \*.cpp -printf "%P ")


# Callable targets

all: _directories


clean:
	rm -rf build


build:


# Internal targets

_directories:
	mkdir -p build