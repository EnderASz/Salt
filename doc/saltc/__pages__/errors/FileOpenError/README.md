# salt::FileOpenError class
```cpp
class FileOpenError;
```
![FileOpenError Inheritance](../../../__assets__/errors/FileOpenError/inheritance.drawio.svg)

FileOpenError class is a represantation of an error occured when SaltC tried open a file at given path to the constructor.
<br>

## Member functions
| Method | Description | Member type |
|--------|-------------|------------------|
| [`(constructor)`](constructor.md) | Constructs the error instance | Public Method |
| `(deconstructor)` | Deconstructs the error instance | Public default deconstructor |
| [`getMessage`](getMessage.md) | Returns the error message | Public Method |
| [`getPath`](getPath.md) | Returns the path of file where error occured | Public Method |
<br>

## Example of use
That example prints out the error message about failed opening file at the path passed into constructor of `FileOpenError` class.
```cpp
eprint(new salt::FileOpenError('path/to/failed/open/file.txt'));
```
<br>

## See also:
+ [SaltC errors](../README.md)
+ [`salt::BaseError` class](../BaseError/README.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)