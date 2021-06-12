# salt::UnspecifiedMainError::getMessage method
```cpp
std::string salt::UnspecifiedMainError::getMessage() override;
```
Returns string error message about failed interpreting command line options and/or arguments.

## Parameters
(None)

## Return Value
Concatenated below string error message and return of [`getHelpRecommendation`](getHelpRecommendation.md) member method.<br>
    
    "Main file to compile is unspecified. Please specify it. "

## Notes
It's an overridden version of [`getMessage`](../CommandLineError/getMessage.md) method inherited from [`salt::CommandLineError`](../CommandLineError/README.md) virtual method.

## See also
+ [SaltC errors](../README.md)
+ [`salt::BaseError::getMessage` method](../../errors/BaseError/getMessage.md)
+ [`salt::UnspecifiedMainError` class](README.md)