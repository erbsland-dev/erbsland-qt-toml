
.. index::
    !single: Namespaces
    !single: erbsland
    !single: erbsland::qt
    !single: erbsland::qt::toml
    !single: el (Shortcut)
    !single: elqt (Shortcut)
    single: Namespaces; erbsland
    single: Namespaces; erbsland::qt
    single: Namespaces; erbsland::qt::toml
    single: Namespaces; el (Shortcut)
    single: Namespaces; elqt (Shortcut)

Namespaces
==========

As this library is part of a large framework, it's nesting of namespaces may look unnecessary complicated. If using in context of larger applications, these additional levels of namespaces provide flexibility and prevent naming conflicts.

The Erbsland Qt TOML interface is placed in the namespace ``erbsland::qt::toml``:

.. doxygennamespace:: erbsland
    :desc-only:

.. doxygennamespace:: erbsland::qt
    :desc-only:

.. doxygennamespace:: erbsland::qt::toml
    :desc-only:

Shortcuts
---------

To simplify your code, shortcuts for the base namespaces are automatically created by default.

.. code-block:: cpp

    namespace el = erbsland;
    namespace elqt = erbsland::qt;

We recommended that you use either use these shortcuts to name the types from the library, or write `using` statements, importing the types you use into the namespace of your application.

.. code-block:: cpp
    :caption: Using the namespace shortcut `elqt` to use the `Parser` class.

    #include <erbsland/qt/toml/Parser.hpp>

    void foo() {
        elqt::toml::Parser parser{};
        // ...
    }

.. code-block:: cpp
    :caption: Importing the `Parser` class, with the `using` statement.

    #include <erbsland/qt/toml/Parser.hpp>

    using elqt::toml::Parser;
    // or `using erbsland::qt::toml::Parser;` when shortcuts are disabled.

    void foo() {
        Parser parser{};
        // ...
    }

Prevent or Redefine the Shortcuts
---------------------------------

As the shortcuts can conflict with existing codes, there are several macro definitions that allow you to disable the shortcuts or redefine them. You will have to set these definition on the compile level for the whole project.

.. c:macro:: ERBSLAND_NO_SHORT_NAMESPACE

    If you set this preprocessor directive, no namespace shortcuts are created by default. You have to set this directive on the compile level for the whole project.

.. c:macro:: ERBSLAND_SHORT_NAMESPACE

    Set this preprocessor directive to overwrite the name that is used for the shortcut for the ``erbsland`` namespace. You have to set this directive on the compile level for the whole project.

.. c:macro:: ERBSLAND_QT_SHORT_NAMESPACE

    Set this preprocessor directive to overwrite the name that is used for the shortcut for the ``erbsland::qt`` namespace. You have to set this directive on the compile level for the whole project.


