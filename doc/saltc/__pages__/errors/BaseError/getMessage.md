# salt::BaseError::getMessage Pure Virtual method
##### Defined by "error.h" 
```cpp
virtual std::string salt::BaseError::getMessage();
```
Returns message of error represented by instance of BaseError child class.

It's pure virtual method and needs to be implemented by inheriting class.

## Parameters
(None)

## Return Value
String error message.

## Implementation Example
This implementation can be use to, pass instance of (in this case) `ValueError` to [`eprint`](<eprint-link-placeholder>) macro in order to print `"Value of 'x' is invalid. Try use 'a' or 'b'."` string as an error message.
```cpp
ValueError::getMessage() {
    return "Value of 'x' is invalid. Try use 'a' or 'b'."
}
```

## See also
+ [SaltC errors](../README.md)
+ [`salt::BaseError` class](../../errors/BaseError/README.md)
+ [`eprint` logging macro](<eprint-link-placeholder>)