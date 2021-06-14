# salt::CustomError::getMessage method
##### Defined by "error.h" 
```cpp
virtual std::string salt::CustomError::getMessage() override;
```
Returns string custom error message given via [`constructor`](constructor.md) of [`salt::CustomError` class](README.md).

## Parameters
(None)

## Return Value
String error message given via [`constructor`](constructor.md) of [`salt::CustomError` class](README.md).

## Notes
It's an overridden version of [`getMessage`](../BaseError/getMessage.md) virtual method inherited from [`salt::BaseError`](../BaseError/README.md).

## See also
+ [SaltC errors](../README.md)
+ [`salt::BaseError::getMessage` method](../../errors/BaseError/getMessage.md)
+ [`salt::CustomError` class](README.md)