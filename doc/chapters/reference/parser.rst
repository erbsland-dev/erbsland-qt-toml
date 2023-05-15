
.. index::
    !single: Parser

=================
Parsing Documents
=================

Create the Parser
=================

TOML documents are parsed with the :cpp:class:`Parser<erbsland::qt::toml::Parser>` class. The parser class stores little state information and is designed to be constructed on the stack using a local variable:

.. code-block:: cpp

    #include <erbsland/qt/toml/Parser.hpp>

    using elqt::toml::Parser;

    void parseConfiguration() {
        Parser parser{};
        // ..
    }

The parser accepts one optional argument with the used TOML :cpp:enum:`Specification<erbsland::qt::toml::Specification>` that must be used for parsing. We recommend using the default constructor, as if new TOML language versions are released, the default value will automatically be set to the latest version.

If you want to limit parsing up to a specific version, or use a newer version that is not already released, you can provide this argument.

.. code-block:: cpp

    #include <erbsland/qt/toml/Parser.hpp>

    using elqt::toml::Parser;
    using elqt::toml::Specification;

    void parseConfiguration() {
        Parser parser{Specification::TOML_1_1}; // Use the TOML 1.1 specification
        // ..
    }


Thread Safety
=============

All functions of the *Erbsland Qt TOML* parser are **reentrant**. Therefore, they can be called from multiple threads simultaneously, as long each thread uses its own instance of :cpp:class:`Parser<erbsland::qt::toml::Parser>`.


The ``Parser`` Class
====================

.. doxygenclass:: erbsland::qt::toml::Parser
    :members:

The ``Value`` Class
===================

.. doxygentypedef:: erbsland::qt::toml::ValuePtr

.. doxygenclass:: erbsland::qt::toml::Value
    :members: