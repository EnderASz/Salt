<h1 align="center">Salt</h1>

<p align="center">
    <img src="https://img.shields.io/badge/Language-C & C++-aF405D?style=flat-square">
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

## The language

Salt is designed to be intuitive, but also with a lot of rules and syntax from other known languages. Each `.salt`
file is called a module, and each module can import another module. 

Let's start with a simple hello world:
```java
public main() {
    print("Hello world");
}
```
As you can see, all functions either start with `public` or `private`, showing their avaibility for other modules.
The main function is always `public`. The `print` function is a global function always imported into your module.

Next, to run the program you need to compile it first. This can be done using `saltc` (or whatever other compiler
you are using). The `-o` flag defines the output of the compiler. After that, run it using `svm`.

```bash
$ saltc ./hello.salt -o hello.scc
$ svm ./hello.scc
```

There we go! Our first program.
```
Hello world
```

### Types

Salt is a **strongly typed language**, meaning type-correctness is enforced by the compiler. To create a variable
which has the number value of 123, you can use an int.
```java
int x = 123;
```
As you can see, it's very simmilar to other typed programming languages, and that's the goal of Salt - to be understandable.

The base types in Salt are: 

| Type     | Description                                    |
|----------|------------------------------------------------|
| `int`    | integer value (−2,147,483,647, +2,147,483,647) |
| `float`  | floating point value                           |
| `string` | dynamic length string                          |
| `bool`   | can be `true` or `false`                       |
| `null`   | no value                                       |


<!-- Installation section -->

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
