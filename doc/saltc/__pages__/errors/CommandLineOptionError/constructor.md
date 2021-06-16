# salt::CommandLineOptionError constructor
##### Defined by "error.h"
```cpp
std::string salt::CommandLineOptionError::CommandLineOptionError(const string& option);
```
Constructs an error caused by failed interpreting command line option.

## Parameters
+ `option` -> Command line option string which can not be interpreted correct.

## Example of use
Below example prints out the error about failed interpreting an `-o` option.
```cpp
eprint(CommandLineOptionError, "-o");
```

## See also
+ [SaltC errors](../README.md)
+ [`salt::FileOpenError` class](README.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)