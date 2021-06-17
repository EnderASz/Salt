# salt::OutOfSourceRangeError::getMessage method
##### Defined by "error.h" 
```cpp
virtual std::string salt::OutOfSourceRangeError::getMessage() override;
```
Returns string error message about attempt to accessing data out of range given via [`constructor`](constructor.md) source file.

## Parameters
(None)

## Return Value
String error message according to the pattern:<br>
    
    "Tried to get data out of range '<source filepath>' source."

## Notes
It's an overridden version of [`getMessage`](../BaseError/getMessage.md) virtual method inherited from [`salt::SourceError`](../SourceError/README.md).

## See also
+ [SaltC errors](../README.md)
+ [`salt::BaseError::getMessage` method](../../errors/BaseError/getMessage.md)
+ [`salt::OutOfSourceRangeError` class](README.md)