# SaltC errors
##### Implemented by "error.h" 

## Basic usage of errors
SaltC errors classes were created to use with [`eprint`](<eprint-link-placeholder>) macro provided by
    [logging module](<logging-module-link-placeholder>) which prints out an error message based on provided parameters and choosen class of error and than closing compiler after it in most safety and easy-to-use possible way.

To use error with logging module you will need to include two saltc header files:
+ [`logging.h`](<logging-module-link-placeholder>)
+ `error.h`

### **Example usage of eprint and any error class.**
```cpp
eprint(new AnyErrorClass(error_parameters...));
```

How you can see it's needed to pass **pointer to error class object** to throw it,
    but it's fully safe because when you calling [`eprint`](<eprint-link-placeholder>) it's cleans that pointer at the moment before closing program.<br>
    So you may not carry about pointer passed to [`eprint`](<eprint-link-placeholder>) and storing it. That's why I used `new` C++ keyword here.


## Error classes
![Error classes diagram](../../__assets__/errors/errors.drawio.svg)

### **Index**
+ [BaseError](BaseError/README.md)
+ [FileOpenError](FileOpenError/README.md)
+ [CommandLineError](CommandLineError/README.md)
+ [UnspecifiedMainError](UnspecifiedMainError/README.md)
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
