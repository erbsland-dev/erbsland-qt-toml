# Erbsland Qt TOML Parser Overview

Welcome to the Erbsland Qt TOML Parser, a highly reliable and secure TOML parser that integrates seamlessly with Qt 6 environments. This parser is fully aligned with the TOML 1.0 specification and is also compatible with the TOML 1.1 draft implemented by this library. Designed with a modern and clean C++ code base, it promises ease of inspection and understanding.

A distinctive feature of this parser is its inherent strict and dependable UTF-8 parsing, which operates independently of the Qt library. Beyond merely returning the data from the parsed document, our parser provides a value structure that includes the start and end locations of each parsed table and value. This feature is instrumental in generating informative and useful error messages.

## Maintenance Status

This library is still maintained for bug fixes, security fixes, and compatibility updates. ErbslandDEV no longer uses Qt, so no new features will be added.

Starting with version 1.3.0, only Qt 6 is supported. Qt 5 support has been removed and will not return; older Qt 5 releases will not receive further updates.

## Key Features

- TOML 1.0 support and compatibility with the TOML 1.1 draft
- No dependencies beyond Qt 6 Core and the C++ standard library
- Robust and secure
- Qt 6 support (C++17, CMake 3.25 or newer)
- User-friendly interface
- Modern, clean C++ code base
- Strict UTF-8 parsing, independent of Qt library

## Getting Started

The *Erbsland Qt TOML* parser is ready to be included as a git submodule in your project and can be linked as a static library. It requires Qt 6 Core and a C++17 compiler. CMake finds Qt 6 directly; set `CMAKE_PREFIX_PATH` to your Qt installation if needed.

Here's a quick peek at how you can use it:

```cpp
#include <erbsland/qt/toml/Parser.hpp>

using namespace elqt::toml;

void readConfiguration() {
    Parser parser{};
    try {
        auto toml = parser.parseFileOrThrow(QStringLiteral("config.toml"));
        // ...
    } catch (const Error &error) {
        // ...
    }
}
```

For a practical illustration, please visit our [Example Project](https://github.com/erbsland-dev/erbsland-qt-toml-example/).

## Documentation

Detailed [documentation](https://erbsland-dev.github.io/erbsland-qt-toml/) can be found here:

👉 [Documentation](https://erbsland-dev.github.io/erbsland-qt-toml/) 👈

## Feedback and Bug Reports

Please use the GitHub issue tracker for bug reports. New feature requests are outside the maintenance scope:

👉 [Report an Issue](https://github.com/erbsland-dev/erbsland-qt-toml/issues) 👈

## Development and Documentation

Use Python 3.14 and install Doxygen with your system package manager.

```sh
python3.14 -m venv .venv
.venv/bin/python -m pip install -r doc/requirements.txt -r tools/development/requirements.txt
.venv/bin/sphinx-build -W --keep-going -b html doc _build/html
```

## License

Copyright © 2023-2026 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch/
According to the copyright terms specified in the file "COPYRIGHT.md".

SPDX-License-Identifier: LGPL-3.0-or-later

This program is free software: you can redistribute it and/or modify it under the terms of the
GNU Lesser General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program.
If not, see <https://www.gnu.org/licenses/>.
