
.. index::
    !single: Location
    !single: LocationRange

Locations
=========

.. cpp:namespace:: erbsland::qt::toml::Value

A unique feature of this TOML parser is its ability to transfer the parsing location from the parsed document to the returned value. Consequently, after parsing, you can retrieve the location from which a value was derived using the :cpp:expr:`Value::locationRange()` method.

This key functionality allows you to report not just logical errors in the configuration, but also communicate the error's location to the user. Both the :cpp:expr:`Location` and :cpp:expr:`LocationRange` classes come equipped with a :cpp:expr:`Location::toString()` method. This method swiftly transforms the location or location range into a format suitable for logging or user display.

.. code-block:: cpp

    #include <erbsland/qt/toml/Value.hpp>
    #include <iostream>

    using namespace elqt::toml;

    void useConfiguration(const ValuePtr &rootTable) {
        auto ports = rootTable->arrayValue(QStringLiteral("ports"));
        for (const auto &[key, value] : ports->toTable()) {
            if (value->type() != ValueType::Integer) {
                std::cerr << "The values in the 'ports' table must be integer values. "
                    << value->locationRange().toString().toStdString() << "\n";
                exit(1);
            }
            // ...
        }
    }

