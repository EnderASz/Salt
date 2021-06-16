# salt::UnspecifiedMainError class
##### Defined by "error.h"
```cpp
class UnspecifiedMainError;
```
![UnspecifiedMainError Inheritance](../../../__assets__/errors/UnspecifiedMainError/inheritance.drawio.svg)

UnspecifiedMainError class is a represantation of an error occured because main source file was not specified via command line argument.
<br>

## Member functions
| Method | Description | Member type |
|--------|-------------|------------------|
| `(constructor)` | Constructs the error instance | Public default constructor |
_______________________________________________________________________________
## Inherited from [`salt::CommandLineError`](../CommandLineError/README.md)
| Method | Description | Member type |
|--------|-------------|------------------|
| `(deconstructor)` | Deconstructs the error instance | Public Virtual default deconstructor |
| [`getMessage`](../BaseError/getMessage.md) <sub><sup>[(override)](getMessage.md)</sup></sub> | Returns the error message | Public Virtual method |
| [`getHelpRecommendation`](../CommandLineError/getHelpRecommendation.md) | Returns the string recommendation to look into SaltC command line help page. | Public method |
<br>

## Example of use
That example prints out the error message about not passed main source file path into command line.
```cpp
eprint(UnspecifiedMainError);
```
<br>

## See also:
+ [SaltC errors](../README.md)
+ [`salt::CommandLineError` class](../CommandLineError/README.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)