# salt::FileOpenError::getMessage method
```cpp
std::string salt::FileOpenError::getMessage() override;
```
Returns string error message about failed opening filepath given via [`constructor`](constructor.md) of [`salt::FileOpenError` class](README.md).

## Parameters
(None)

## Return Value
String error message according to the pattern:<br>
    
    "An error occurred when tried to open '<filepath>' file."

## Notes
It's an overridden version of [`getMessage`](../BaseError/getMessage.md) method inherited from [`salt::BaseError`](../BaseError/README.md) virtual method.

## See also
+ [SaltC errors](../README.md)
+ [`salt::BaseError::getMessage` method](../../errors/BaseError/getMessage.md)
+ [`salt::FileOpenError` class](README.md)