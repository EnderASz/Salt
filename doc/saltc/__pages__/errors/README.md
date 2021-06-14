# SaltC errors

## Basic usage of errors
SaltC errors classes were created to use with [`eprint`](<eprint-link-placeholder>) macro provided by
    [logging module](<logging-module-link-placeholder>) which prints out an error message based on provided parameters and choosen class of error and than closing compiler after it in most safety and easy-to-use possible way.

To use error with logging module you will need to include only one saltc header file: [`logging.h`](<logging-module-link-placeholder>).

### **Example usage of eprint and any error class.**
```cpp
eprint(AnyErrorClassName, error_parameters...);
```

How you can see it's needed to pass only error class name and their constructor parameters to throw it. Everything is safe in this beacause eprint macro takes responsibility to create an error object from pointed class name and given parameters.


## Error classes
##### Defined by "error.h"
![Error classes diagram](../../__assets__/errors/errors.drawio.svg)

### **Index**
+ [BaseError](BaseError/README.md)
+ [CustomError](CustomError/README.md)
+ [FileOpenError](FileOpenError/README.md)
+ [CommandLineError](CommandLineError/README.md)
+ [UnspecifiedMainError](UnspecifiedMainError/README.md)
+ [CommandLineOptionError](CommandLineOptionError/README.md)
+ [UnrecognizedOptionError](UnrecognizedOptionError/README.md)
+ [SourceError](SourceError/README.md)
+ [OutOfSourceRangeError](OutOfSourceRangeError/README.md)
+ [InSourceError](InSourceError/README.md)
+ [UnexpectedTokenError](UnexpectedTokenError/README.md)
+ [UnknownTokenError](UnknownTokenError/README.md)
+ [UnclosedCommentError](UnclosedCommentError/README.md)
+ [UnclosedStringError](UnclosedStringError/README.md)
+ [InvalidLiteralError](InvalidLiteralError/README.md)
+ [TooLongLiteralError](TooLongLiteralError/README.md)
