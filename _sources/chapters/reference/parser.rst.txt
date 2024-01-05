.. index::
    !single: Parser

=================
Parsing Documents
=================

.. index::
    single: Instantiate the Parser

Instantiate the Parser
======================

To parse TOML documents, utilise the :cpp:class:`Parser<erbsland::qt::toml::Parser>` class. This class is lightweight and designed to be instantiated as a local variable on the stack:

.. code-block:: cpp

    #include <erbsland/qt/toml/Parser.hpp>

    using namespace elqt::toml;

    void parseConfiguration() {
        Parser parser{};
        // ..
    }

The parser's constructor accepts an optional argument indicating the TOML :cpp:enum:`Specification<erbsland::qt::toml::Specification>` to be used. We advise sticking with the default constructor, as it automatically adopts the latest TOML version whenever a new release occurs.

If you need to restrict parsing to a specific TOML version or use an unreleased version, you can specify this using the optional argument:

.. code-block:: cpp

    #include <erbsland/qt/toml/Parser.hpp>

    using namespace elqt::toml;

    void parseConfiguration() {
        Parser parser{Specification::TOML_1_1}; // Use the TOML 1.1 specification
        // ..
    }

.. index::
    !single: Parse Files
    single: parseFileOrThrow()

File Parsing
============

.. cpp:namespace:: erbsland::qt::toml::Parser

The :cpp:texpr:`parseFileOrThrow()` method is used to read and interpret TOML configuration files from disk. It requires one parameter: the absolute path to the file to be parsed.

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

The parser will attempt to open, read, and parse the specified file. If any issues arise during this process, an exception is thrown.

The :cpp:texpr:`Error` object of the exception provides valuable information about the problem that prevented successful parsing. For more details, see the :ref:`error-handling` chapter.

Parsing Strings and Byte Data
=============================

If the TOML file has already been read by another component of your software, you can parse TOML data from a ``QString`` or ``QByteArray`` using the :cpp:expr:`parseStringOrThrow()` and :cpp:expr:`parseDataOrThrow()` methods, respectively.

.. code-block:: cpp

    #include <erbsland/qt/toml/Parser.hpp>

    using namespace elqt::toml;

    void parseConfiguration(const QString &tomlText) {
        Parser parser{};
        try {
            auto toml = parser.parseStringOrThrow(tomlText);
            // Process the TOML document here.
        } catch (const Error &error) {
            // Handle any parsing errors here.
        }
    }

These methods work similarly to :cpp:expr:`parseFileOrThrow()`, with the key difference being that they won't throw an :cpp:expr:`Error::Type::IO` error.

.. note::

    Although :cpp:expr:`parseFileOrThrow()` and :cpp:expr:`parseDataOrThrow()` utilise the same strict built-in UTF-8 decoder, :cpp:expr:`parseStringOrThrow()` initially relies on the UTF-16 encoding inherent to the string. Any encoding errors will be detected when these values are read from the string.

    Therefore, even with a ``QString``, you may encounter :cpp:expr:`Error::Type::Encoding` errors. However, these errors would be due to issues with UTF-16 encoding, not UTF-8.

Thread Safety
=============

Every function within the *Erbsland Qt TOML* parser is **reentrant**, meaning it can be safely executed concurrently from multiple threads. However, it's essential that each thread operates on its own :cpp:class:`Parser<erbsland::qt::toml::Parser>` instance.

To put it in simpler terms, a reentrant function is one that can be paused partway through its execution, called again (reentered), and then safely resumed from where it left off, without causing any conflicts or inconsistencies. This characteristic makes reentrant functions particularly useful in multi-threaded environments.

With *Erbsland Qt TOML* parser, you can have multiple threads parsing different TOML documents simultaneously, as long as each thread uses its own :cpp:expr:`Parser` instance.

Interacting with the Parsed Output
==================================

The parser generates the output as a nested hierarchy of :cpp:expr:`Value` objects. The returned object represents the root table of the document, and it always of the :cpp:expr:`ValueType::Table` type.

.. cpp:namespace:: erbsland::qt::toml::ValueType

Each value in this structure can be of the following types :cpp:expr:`Integer`, :cpp:expr:`Float`, :cpp:expr:`Boolean`, :cpp:expr:`String`, :cpp:expr:`Time`, :cpp:expr:`Date`, :cpp:expr:`DateTime`, :cpp:expr:`Table`, and :cpp:expr:`Array`. For the latter two types, which are often checked, there are two dedicated functions: :cpp:expr:`Value::isTable()` and :cpp:expr:`Value::isArray()`.

Instead of returning a value directly, the parser returns a :cpp:expr:`std::shared_ptr<Value>` pointing to the instance. This approach guarantees easy duplication and transfer of the parsed document's structure to different parts of your application, eliminating the need for manual memory management. The library also defines :cpp:expr:`ValuePtr` you should use in declarations for this shared pointer.

Accessing Individual Values using Keys
--------------------------------------

.. cpp:namespace:: erbsland::qt::toml::Value

We advise using the ``<type>Value(keypath)`` methods to access individual values, as demonstrated by the :cpp:expr:`stringValue()` function in the following example.

.. code-block:: cpp

    #include <erbsland/qt/toml/Value.hpp>

    using namespace elqt::toml;

    void useConfiguration(const ValuePtr &rootTable) {
        auto ipAddress = rootTable->stringValue(QStringLiteral("server.ip-address"), QStringLiteral("127.0.0.1"));
        // ...
    }

These methods can process either a single key or a key path where each key is separated by a dot (``.``). If the key doesn't exist or doesn't match the expected type, the function will return the default value specified as the second argument.

Keys that contain a dot in the name are not supported by this convenience methods, if you have to work with unusual keys like this, there is the special method `valueFromKey()` for this case.

Array Iteration
---------------

To iterate over an array, you can use the :cpp:expr:`begin()` and :cpp:expr:`end()` methods, utilize one of the standard library's algorithms, or employ the ``for`` loop syntax directly.

.. code-block:: cpp

    #include <erbsland/qt/toml/Value.hpp>

    using namespace elqt::toml;

    void useConfiguration(const ValuePtr &rootTable) {
        auto serverNames = rootTable->arrayValue(QStringLiteral("server_names"));
        // this directly accesses the vector within the value.
        for (const auto &entry : *serverNames) {
            // ...
        }
    }

Alternatively, you can first convert the table value into a :cpp:expr:`std::vector`. In this scenario, as illustrated in the following example, the ``serverNames`` list becomes a copy of the vector contained in the value instance.

.. code-block:: cpp

    #include <erbsland/qt/toml/Value.hpp>

    using namespace elqt::toml;

    void useConfiguration(const ValuePtr &rootTable) {
        // this creates a copy of the vector stored in the value.
        auto serverNames = rootTable->arrayValue(QStringLiteral("server_names")).toArray();
        for (const auto &entry : serverNames) {
            // ...
        }
    }

Retrieving All Keys and Values of a Table
-----------------------------------------

There are two primary methods to access all keys and corresponding values in a table value. The first method involves using the :cpp:expr:`tableKeys()` function, followed by the :cpp:expr:`valueForKey()` function to fetch the value associated with a particular key.

.. code-block:: cpp

    #include <erbsland/qt/toml/Value.hpp>

    using namespace elqt::toml;

    void useConfiguration(const ValuePtr &rootTable) {
        auto ports = rootTable->arrayValue(QStringLiteral("ports"));
        for (const auto &key : ports->tableKeys()) {
            auto value = ports->valueForKey(key);
            // ...
        }
    }

Alternatively, you can initially convert the value into a :cpp:expr:`std::unordered_map`, then iterate over the map's entries as demonstrated in the following example:

.. code-block:: cpp

    #include <erbsland/qt/toml/Value.hpp>

    using namespace elqt::toml;

    void useConfiguration(const ValuePtr &rootTable) {
        auto ports = rootTable->arrayValue(QStringLiteral("ports"));
        for (const auto &[key, value] : ports->toTable()) {
            // ...
        }
    }
