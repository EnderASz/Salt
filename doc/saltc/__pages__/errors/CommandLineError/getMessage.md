# salt::CommandLineError::getMessage method
##### Defined by "error.h"
```cpp
virtual std::string salt::CommandLineError::getMessage() override;
```
Returns string error message about failed interpreting command line options and/or arguments.

## Parameters
(None)

## Return Value
Concatenated below string error message and return of [`getHelpRecommendation`](getHelpRecommendation.md) member method.<br>

    "An error occured in command line. "

## Notes
It's an overridden version of [`getMessage`](../BaseError/getMessage.md) virtual method inherited from [`salt::BaseError`](../BaseError/README.md) virtual method.

## See also
+ [SaltC errors](../README.md)
+ [`salt::BaseError::getMessage` method](../../errors/BaseError/getMessage.md)
+ [`salt::CommandLineError` class](README.md)