
.. cpp:namespace:: erbsland::qt::toml::Parser

.. index::
    !single: Error
    single: Error Handling

.. _error-handling:

==============
Error Handling
==============

Erbsland DEV libraries employ two strategies for error handling: *Default behaviour* and clearly labeled *exceptions*. This approach has been adopted to counter the common tendency of users to overlook error handling in their code.

Default Behaviour
=================

Most method calls and core functionalities will resort to a default behaviour when encountering errors.

This may seem counterintuitive at first, given that default behaviours have their own downsides. However, let me explain why I favour this approach.

In well-tested production code, accessing an index in a string that is out of range is unlikely. Yet, as testing can only cover a limited range of scenarios, there's always the risk that such an error may remain undetected in the code. A popular approach would be to raise an exception in this situation, which would compel the user to check for out-of-range exceptions each time they use the index operator. Realistically, no one would do this unless the language forces them to. And even if the language mandates exception handling, often it is either overlooked or leads to a software crash.

A fallback to default behaviour in error situations will also lead to unexpected situations, but the incorrect behaviour can be detected at the interface of another module and fixed there. For example, if you call the :cpp:expr:`Value::toString()` method on a value that is no string, you will get an empty string in return. If, at a later stage of your software, this string must not be empty, you will handle the problem there - where it matters.

This approach offers flexibility to the user. If system uptime is more important than the occasional hiccup in data processing, the code can be written in a compact way, focusing on error handling at the interfaces.

If, however, security and strict data verification is a priority, additional checks are available in the API and can be easily added to the code to detect errors early. So if having the correct type of a value is important, one could call :cpp:expr:`Value::type()` first and check if the type of the value is :cpp:expr:`ValueType::String` before converting it into a string.

Exceptions
==========

Exceptions are used when error handling is desired by the user and/or they wish to centralize the handling somewhere in the code. A good example is parsing documents. This library provides two methods for situations like this: one that employs default behaviour and another that throws an exception. The latter *always* uses the suffix ``...OrThrow``` to indicate that an exception is thrown in case of an error.

.. code-block:: cpp

    #include <erbsland/qt/toml/Parser.hpp>

    using namespace elqt::toml;

    void parseConfiguration() {
        auto path = QStringLiteral("/path/to/config.toml");
        Parser parser{};
        try {
            auto toml = parser.parseFileOrThrow(path);
            // Process the TOML document here.
        } catch (const Error &error) {
            // Handle any parsing errors here.
        }
    }

