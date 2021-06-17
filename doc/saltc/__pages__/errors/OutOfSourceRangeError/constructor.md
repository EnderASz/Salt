# salt::OutOfSourceRangeError constructor
##### Defined by "error.h" 
```cpp
std::string salt::OutOfSourceRangeError::OutOfSourceRangeError(
    const SourceFile& source_file);
```
Constructs an error pointing to the given source file where SaltC was trying to access data from out of range.

## Parameters
+ `source_file` -> [`SourceFile`](<source-file-link-placeholder>) object where the error occurred.

## Example of use
That example prints out the error message about failed access to out of range data from source file at path `"path/to/salt/file.salt"`.
```cpp
salt::SourceFile source("path/to/salt/file.salt");
if(/*if index which we want to access is not in range*/) {
    eprint(SourceError, source);
}
```
<br>

## See also
+ [SaltC errors](../README.md)
+ [`salt::OutOfSourceRangeError` class](README.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)