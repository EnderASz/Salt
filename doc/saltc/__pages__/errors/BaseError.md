# BaseError class
```cpp
    class BaseError {
    public:
        virtual std::string getMessage() = 0;
        virtual ~BaseError() = default;
    }
```
BaseError class is base error abstract class identificating that the instance of a child of this class is an error.

Any error which would be printed out by [logging module](<logging-module-link-placeholder>) via [`eprint`](<eprint-link-placeholder>) function needs to be a child of this class.

It can not be used to print the error by itself.

Always when instance of child of this class is passes to [`eprint`](<eprint-link-placeholder>) function, it prints string returned by `getMessage()` method.

```Each child of this class must implements getMessage() method.``` 

## Example of use
```cpp
    class NewError : BaseError {
    public:
        std::string getMessage() {
            return "An error message."
        }
    }
```

Also see:
+ [SaltC errors](../errors.md)