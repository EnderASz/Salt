# salt::BaseError class
```cpp
class BaseError;
```
`BaseError` class is base error abstract class identificating that the instance of a child of this class is an error.

Any error which would be printed out by [logging module](<logging-module-link-placeholder>) via [`eprint`](<eprint-link-placeholder>) macro needs to be a child of this class.

It can not be used as [`eprint`](<eprint-link-placeholder>) parameter to print the error by itself.

Always when instance of child of this class is passes to [`eprint`](<eprint-link-placeholder>) macro, it prints string returned by [`getMessage()`](BaseError/getMessage.md) method.

`Each child of this class must implements` [`getMessage()`](BaseError/getMessage.md) ```method.``` 
<br>

## Member functions
| Method | Description | Member type |
|--------|-------------|------------------|
| `(deconstructor)` | Destructs the error instance | Public Virtual default deconstructor |
| [`getMessage`](BaseError/getMessage.md) | Returns the error message | Public Pure Virtual Method |
<br>

## Example of use
```cpp
class NewError : salt::BaseError {
public:
    std::string getMessage() {
        return "An error message."
    }
}
```
<br>

## See also:
+ [SaltC errors](../README.md)
+ [`getMessage` public member virtual function](getMessage.md)
+ [`eprint` logging module macro](<eprint-link-placeholder>)