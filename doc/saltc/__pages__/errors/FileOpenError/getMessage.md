# salt::FileOpenError::getMessage method
```cpp
virtual std::string salt::FileOpenError::getMessage() override;
```
Returns string error message about failed opening filepath given via [`constructor`](constructor.md) of [`salt::FileOpenError` class](README.md).

## Parameters
(None)

## Return Value
String error message according to the pattern:<br>
    
    "An error occurred when tried to open '<filepath>' file."

## See also
+ [SaltC errors](../README.md)
+ [`salt::BaseError::getMessage` method](../../errors/BaseError/getMessage.md)
+ [`salt::FileOpenError` class](README.md)