# salt::CommandLineError::getMessage method
```cpp
std::string salt::CommandLineError::getMessage() override;
```
Returns string error message about failed interpreting command line options and/or arguments.

## Parameters
(None)

## Return Value
Concatenated below string error message and return of [`getHelpRecommendation`](getHelpRecommendation.md) member method.<br>
    
    "An error occured in command line. "

## See also
+ [SaltC errors](../README.md)
+ [`salt::BaseError::getMessage` method](../../errors/BaseError/getMessage.md)
+ [`salt::CommandLineError` class](README.md)