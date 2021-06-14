# salt::SourceError constructor
##### Defined by "error.h" 
```cpp
std::string salt::SourceError::SourceError(const SourceFile& source_file);
```
Constructs an error pointing to the given source file where the error occurred.

## Parameters
+ `source_file` -> [`SourceFile`](<source-file-link-placeholder>) object where the error occurred.

## Example of use
That example prints out the error message about something failed with Salt source file at path `"path/to/salt/file.salt"`.
```cpp
salt::SourceFile source("path/to/salt/file.salt");
eprint(SourceError, source);
```
<br>

## See also
+ [SaltC errors](../README.md)
+ [`salt::SourceError` class](README.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)