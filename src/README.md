# Code Documentation

Below all the documentation regarding coding standards.

### Syntax

The project use's the modern google naming convention with few modifications. If you are unsure about how to name something just look at the code.

### Code docs flags

Docs flags are basically comments in the code documentation of functions, classes, variables, etc... that helps during development to easily understand the behaviour of a piece of code.

NOTE: Once they are added to a piece of code, the code needs to maintain that standard or they have to be removed.

**Full list of all flags**

- [SINGLETON] -> A class having this flag is a singleton instance.
- [THREADSAFE] -> A class, function or variable that can be considered threadsafe.
- [PRIVATE_USAGE] -> A class, function or variable that is exposed only for private usage therefore it should not be used if not for strict internal implementation.

Flags example

``` cpp
    /*
     * [SINGLETON] [THREADSAFE] Manages interactions with views. Spawns, destroys
     * and render them.
     */
    class RViewPool {
        // Singleton thread safe code here
    } 
```
