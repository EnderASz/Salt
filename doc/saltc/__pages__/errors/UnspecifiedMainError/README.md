# salt::UnspecifiedMainError class
```cpp
class UnspecifiedMainError;
```
![UnspecifiedMainError Inheritance](../../../__assets__/errors/UnspecifiedMainError/inheritance.drawio.svg)

UnspecifiedMainError class is a represantation of an error occured because main source file was not specified via command line argument.
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
That example prints out the error message about not passed main source file path into command line.
```cpp
eprint(new salt::UnspecifiedMainError());
```
<br>

## See also:
+ [SaltC errors](../README.md)
+ [`salt::CommandLineError` class](../CommandLineError/README.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)