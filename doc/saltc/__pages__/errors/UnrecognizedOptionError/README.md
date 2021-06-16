# salt::UnrecognizedOptionError class
##### Defined by "error.h"
```cpp
class UnrecognizedOptionError;
```
![UnrecognizedOptionError Inheritance](../../../__assets__/errors/UnrecognizedOptionError/inheritance.drawio.svg)

UnrecognizedOptionError class is a represantation of an error occured because any command line option or argument cannot be recognized.
<br>

## Member functions
| Method | Description | Member type |
|--------|-------------|------------------|
| [`(constructor)`](constructor.md) | Constructs the error instance | Public constructor |
_______________________________________________________________________________
## Inherited from [`salt::CommandLineError`](../CommandLineError/README.md)
| Method | Description | Member type |
|--------|-------------|------------------|
| `(deconstructor)` | Deconstructs the error instance | Public Virtual default deconstructor |
| [`getMessage`](../BaseError/getMessage.md) <sub><sup>[(override)](getMessage.md)</sup></sub> | Returns the error message | Public Virtual method |
| [`getHelpRecommendation`](../CommandLineError/getHelpRecommendation.md) | Returns the string recommendation to look into SaltC command line help page. | Public method |
| [`getOption`](../CommandLineOptionError/getOption.md) | Returns string command line option which cannot be recognized | Public method |
<br>

## Example of use
That example prints out the error message about unrecognized command line option `'--IAmNotValidOption'`.
```cpp
eprint(UnrecognizedOptionError, "--IAmNotValidOption");
```
<br>

## See also:
+ [SaltC errors](../README.md)
+ [`salt::CommandLineError` class](../CommandLineError/README.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)