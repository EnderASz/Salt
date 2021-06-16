# salt::SourceError::getMessage method
##### Defined by "error.h"
```cpp
virtual std::string salt::SourceError::getMessage() override;
```
Returns string error message about non specified failure during interacting with the [`Salt source file`](<source-file-link-placeholder>) given via [`constructor`](constructor.md).

## Parameters
(None)

## Return Value
String error message according to the pattern:<br>

    "An error occured in '<source filepath>' source."

## Notes
It's an overridden version of [`getMessage`](../BaseError/getMessage.md) virtual method inherited from [`salt::BaseError`](../BaseError/README.md).

## See also
+ [SaltC errors](../README.md)
+ [`salt::BaseError::getMessage` method](../../errors/BaseError/getMessage.md)
+ [`salt::SourceError` class](README.md)