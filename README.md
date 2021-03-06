<h1 align="center">Salt</h1>

<p align="center">
    <img src="https://img.shields.io/badge/Language-C & C++-aF405D?style=flat-square">
    <img src="https://img.shields.io/github/last-commit/EnderASz/Salt?label=Last%20commit&style=flat-square">
    <img src="https://img.shields.io/tokei/lines/github/EnderASz/Salt?label=Total%20lines&style=flat-square">
</p>


Welcome to Salt. This is a compiled, interpreted programing language running in a virtual machine enviroment.

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

To run a Salt program you need to compile it first. This can be done using `saltc` (or whatever other compiler
you are using). The `-o` flag defines the output of the compiler. To run a compiled program use `svm`.

```bash
$ saltc ./hello.salt -o hello.scc
$ svm ./hello.scc
```

There we go! Our first program.
```
Hello world
```

### Types

Salt is a **strongly typed language** thus type-correctness is enforced by the compiler. To create a variable
which has the number value of 123, you can use an int.
```java
int x = 123;
```
As you can see, the syntax is very simmilar to other typed programming languages, and that's the goal of Salt - to be understandable.

The base types in Salt are: 

| Type     | Description                                                             |
|----------|-------------------------------------------------------------------------|
| `int`    | integer value (−9,223,372,036,854,775,807 to 9,223,372,036,854,775,808) |
| `float`  | floating point value                                                    |
| `string` | dynamic length string                                                   |
| `bool`   | can be `true` or `false`                                                |
| `null`   | no value                                                                |


<!-- Installation section -->

## Installation

### Linux

Clone the repository and run:
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

WIP, for now you can use WSL


## Contributing

If you want to contribute, you must always follow these rules:

* Keep the style the same as the code around you
* Everything must be atleast in English
* Each source code file must contain the copyright header
* Don't step on other's people toes - make your own branch
* When changing something in an existing file, add your name to the copyright header
* Keep the same format of commit messages (`subject: change`)
