# salt::UnrecognizedOptionError::getMessage method
##### Defined by "error.h"
```cpp
virtual std::string salt::UnrecognizedOptionError::getMessage() override;
```
Returns string error message about unrecognized command line option or argument.

## Parameters
(None)

## Return Value
Concatenated string error message according to the below pattern and return of [`getHelpRecommendation`](../CommandLineError/getHelpRecommendation.md) member method.<br>

    "Unrecognized command line option '<given via constructor option/argument>'. "

## Notes
It's an overridden version of [`getMessage`](../BaseError/getMessage.md) virtual method inherited from [`salt::CommandLineOptionError`](../CommandLineOptionError/README.md).

## See also
+ [SaltC errors](../README.md)
+ [`salt::BaseError::getMessage` method](../../errors/BaseError/getMessage.md)
+ [`salt::UnrecognizedOptionError` class](README.md)