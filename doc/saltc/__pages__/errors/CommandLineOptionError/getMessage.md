# salt::CommandLineOptionError::getMessage method
##### Defined by "error.h" 
```cpp
virtual std::string salt::CommandLineOptionError::getMessage() override;
```
Returns string error message about failed interpreting the command line option given via [`constructor`](constructor.md).

## Parameters
(None)

## Return Value
Concatenated string error message according to the below pattern and return of [`getHelpRecommendation`](../CommandLineError/getHelpRecommendation.md) member method.<br>
    
    "A command line option '<given via constructor option>' cannot be interpreted successfully. "

## Notes
It's an overridden version of [`getMessage`](../BaseError/getMessage.md) virtual method inherited from [`salt::CommandLineError`](../CommandLineError/README.md).

## See also
+ [SaltC errors](../README.md)
+ [`salt::BaseError::getMessage` method](../../errors/BaseError/getMessage.md)
+ [`salt::UnrecognizedOptionError` class](README.md)