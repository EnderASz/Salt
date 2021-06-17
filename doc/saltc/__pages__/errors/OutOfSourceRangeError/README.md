# salt::OutOfSourceRangeError class
##### Defined by "error.h" 
```cpp
class OutOfSourceRangeError;
```
![OutOfSourceRangeError Inheritance](../../../__assets__/errors/OutOfSourceRangeError/inheritance.drawio.svg)

OutOfSourceRangeError class is a represantation of an error occured because SaltC was trying to access data out of given via [`constructor`](constructor.md) source file range.
<br>

## Member functions
| Method | Description | Member type |
|--------|-------------|------------------|
| [`(constructor)`](constructor.md) | Constructs the error instance | Public constructor |
_______________________________________________________________________________
## Inherited from [`salt::SourceError`](../SourceError/README.md)
| Method | Description | Member type |
|--------|-------------|------------------|
| `(deconstructor)` | Deconstructs the error instance | Public Virtual default deconstructor |
| [`getMessage`](../BaseError/getMessage.md) <sub><sup>[(override)](getMessage.md)</sup></sub> | Returns the error message | Public Virtual method |
| [`getSource`](../SourceError/getSource.md) | Returns a pointer to the Salt source file object affected by this error | Public method |
<br>

## Example of use
That example prints out the error message about failed access to out of range data from source file at path `"path/to/salt/file.salt"`.
```cpp
salt::SourceFile source("path/to/salt/file.salt");
if(/*if index which we want to access is not in range*/) {
    eprint(SourceError, source);
}
```
<br>

## See also:
+ [SaltC errors](../README.md)
+ [`salt::BaseError` class](../BaseError/README.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)