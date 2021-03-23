<h1 align="center">Salt</h1>

<p align="center">
    <img src="https://img.shields.io/badge/Language-C & C++-aF405D?style=flat-square">
    <img src="https://img.shields.io/badge/Version-0.1-00ccff?style=flat-square">
    <img src="https://img.shields.io/github/last-commit/EnderASz/Salt?label=Last%20commit&style=flat-square">
    <img src="https://img.shields.io/tokei/lines/github/EnderASz/Salt?label=Total%20lines&style=flat-square">
</p>


This is the Salt programming language. This is a compiled, interpreted language in a virtual machine.

Directories:
- `lib` - standard library of Salt
- `doc` - md, txt & html documentation
- `saltc` - salt compiler 
- `svm` - salt virtual machine 

<br>

## Installation

### Linux

Just run this:
```
$ ./install.sh
```
This script:
* Builds the compiler & virtual machine
* Creates a salt home in `~/.local/salt`
* Adds `$SALT_HOME` to `~/.bashrc`
* Adds `$SALT_HOME/bin` to `$PATH`
* Moves compiled programs, `lib` and `doc` to salt home
* Make clean for both `saltc` and `svm`

### Windows

Good luck
