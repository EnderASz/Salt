# salt::FileOpenError constructor
##### Defined by "error.h" 
```cpp
std::string salt::FileOpenError::FileOpenError(const std::string& filepath);
```
Constructs a failed open file error pointing to the given filepath.

## Parameters
+ `filepath` -> Path of file which opening failed

## Example of use
Below example prints out the error about failed opening file at `'path/to/my/file/test.txt'` if this file cannot be opened successfully.
```cpp
std::string = "path/to/my/file/test.txt"
std::ifstream file(filepath.c_str());
if(!file.good()) {
    eprint(FileOpenError, filepath);
}
```

## See also
+ [SaltC errors](../README.md)
+ [`salt::FileOpenError` class](README.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)