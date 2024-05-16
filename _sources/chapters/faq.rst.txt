
Frequently Asked Questions
==========================

Why does this parser use exceptions, even though Qt does not?
    This is a matter of philosophical approach. We maintain that the judicious use of exceptions can result in cleaner and easier-to-maintain code compared to return codes. When a library is deeply integrated into a larger process, which needs to halt on any error, it's often simpler to handle exceptions along the way and catch them at a designated point to terminate the program. Avoiding exceptions altogether can result in more convoluted and difficult-to-understand code.

    In this library, all functions (however few) that throw exceptions are clearly designated with the ``OrThrow`` suffix. Therefore, if you use these functions, you must also ensure that you catch any exceptions thrown by them, as outlined in the API documentation.

    Wherever practical, we provide a second set of functions offering the same functionality but returning a default value instead of throwing an exception.

Why aren't the parser's returned values protected with ``const``?
    This too is a matter of our philosophical approach. While the use of constant types has its advantages, and we employ ``const`` where it makes sense, especially to avoid potential pitfalls for the interface user, we strive to keep the interface as flexible and open as possible based on our understanding of the library's usage.

    For instance, a user might want to transform or modify the parsing result. Returning a const result would necessitate complex cloning logic, etc.
