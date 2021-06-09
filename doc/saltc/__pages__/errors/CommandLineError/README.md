# salt::CommandLineError class
```cpp
class CommandLineError;
```
![CommandLineError Inheritance](../../../__assets__/errors/CommandLineError/inheritance.drawio.svg)

CommandLineError class is a represantation of an error occured when SaltC cannot properly interpret command line options and/or arguments.
<br>

## Member functions
| Method | Description | Member type |
|--------|-------------|------------------|
| [`(constructor)`](constructor.md) | Constructs the error instance | Public default constructor |
| `(deconstructor)` | Deconstructs the error instance | Public default deconstructor |
| [`getMessage`](getMessage.md) | Returns the error message | Public Method |
| [`getHelpRecomendation`](getHelpRecomendation.md) | Returns the string recommendation to look into SaltC help page | Public Method |
<br>

## Example of use
That example prints out the error message about a mistake in command line.
```cpp
eprint(new salt::CommandLineError());
```
<br>

## See also:
+ [SaltC errors](../README.md)
+ [`salt::BaseError` class](../BaseError/README.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)