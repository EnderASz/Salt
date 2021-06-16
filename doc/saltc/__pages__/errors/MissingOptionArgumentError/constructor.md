# salt::UnrecognizedOptionError constructor
##### Defined by "error.h"
```cpp
std::string salt::UnrecognizedOptionError::UnrecognizedOptionError(const string& option);
```
Constructs an error about missing command line option argument.

## Parameters
+ `option` -> Command line option string which requires and didn't get argument.

## Example of use
That example prints out the error message about missing argument for `--output` command line option.
```cpp
eprint(MissingOptionArgumentError, "--output");
```

## See also
+ [SaltC errors](../README.md)
+ [`salt::FileOpenError` class](README.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)