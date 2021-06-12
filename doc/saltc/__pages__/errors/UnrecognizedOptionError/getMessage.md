# salt::UnrecognizedOptionError::getMessage method
```cpp
std::string salt::UnrecognizedOptionError::getMessage() override;
```
Returns string error message about unrecognized command line option or argument.

## Parameters
(None)

## Return Value
Concatenated string error message according to the below pattern and return of [`getHelpRecommendation`](../CommandLineError/getHelpRecommendation.md) member method.<br>
    
    "Unrecognized command line option '<given via constructor option/argument>'. "

## Notes
It's an overridden version of [`getMessage`](../CommandLineError/getMessage.md) method inherited from [`salt::CommandLineError`](../CommandLineError/README.md) virtual method.

## See also
+ [SaltC errors](../README.md)
+ [`salt::BaseError::getMessage` method](../../errors/BaseError/getMessage.md)
+ [`salt::UnrecognizedOptionError` class](README.md)