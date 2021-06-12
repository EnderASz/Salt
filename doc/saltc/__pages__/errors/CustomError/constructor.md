# salt::CustomError constructor
```cpp
std::string salt::CustomError::CustomError(const string& message);
```
Constructs an error with given message.

## Parameters
+ `message` -> An custom error message.

## Example of use
Below example prints out the error with `'An error occurred when xxx in zzz!'` message and exits the program.
```cpp
eprint(new salt::CustomError("An error occurred when xxx in zzz!"));
```

## See also
+ [SaltC errors](../README.md)
+ [`salt::CustomError` class](README.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)