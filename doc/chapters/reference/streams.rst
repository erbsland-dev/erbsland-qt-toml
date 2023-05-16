
.. cpp:namespace:: erbsland::qt::toml

.. index::
    !single: InputStream

=============
Input Streams
=============

If you like to stream your document from another source than files, strings or byte data, you can subclass :cpp:expr:`InputStream` and implement your own streaming class.

- Your subclass must use the :cpp:expr:`InputStream::Type::Custom` type and implement the :cpp:expr:`InputStream::atEnd()` and :cpp:expr:`InputStream::readCharOrThrow()` methods.
- You code must read one unicode character at a time and return it. If there is any problem, the method :cpp:expr:`InputStream::readCharOrThrow()` must throw an exception using the :cpp:expr:`Error` class.

