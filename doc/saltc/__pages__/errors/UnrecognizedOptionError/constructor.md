# salt::UnrecognizedOptionError constructor
##### Defined by "error.h"
```cpp
std::string salt::UnrecognizedOptionError::UnrecognizedOptionError(const string& option);
```
Constructs a unrecognized command line option error with given option string.

## Parameters
+ `option` -> Command line option string which can not be recognized.

## Example of use
Below example prints out the error about unrecognized `arg` option if it's not according to any compared predefined option.
```cpp
while(!args.empty()) {
    if(/* arg compare condition */) {
        /* Code when arg compare returns true */
    }
    // ...
    else {
        eprint(UnrecognizedOptionError, arg);
    }
}
```

## See also
+ [SaltC errors](../README.md)
+ [`salt::FileOpenError` class](README.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)