
=======
Preface
=======

Welcome to the documentation of the *Erbsland Qt TOML Parser*, a highly reliable and secure TOML parser that integrates seamlessly with Qt 6 environments. This parser is fully aligned with the TOML 1.0 specification and is also compatible with the TOML 1.1 draft implemented by this library. Designed with a modern and clean C++ code base, it promises ease of inspection and understanding.

A distinctive feature of this parser is its inherent strict and dependable UTF-8 parsing, which operates independently of the Qt library. Beyond merely returning the data from the parsed document, our parser provides a value structure that includes the start and end locations of each parsed table and value. This feature is instrumental in generating informative and useful error messages.

Key Features
============

- TOML 1.0 support and compatibility with the TOML 1.1 draft
- No dependencies beyond Qt 6 Core and the C++ standard library
- Robust and secure
- Compatibility with Qt 6
- User-friendly interface
- Modern, clean C++ code base
- Strict UTF-8 parsing, independent of Qt library

Maintenance
===========

This library is still maintained for bug fixes, security fixes, and compatibility
updates. ErbslandDEV no longer uses Qt, so no new features will be added.

Version 1.3.0 removes Qt 5 support. Qt 5 support will not return, and earlier
releases for Qt 5 will not receive further updates.

Design Goals
============

- No additional dependencies
  - uses the C++ standard library
  - uses a few classes from the Qt 6 Core library
- Secure to be used for TOML from untrusted sources
  - no buffer overflows
  - no hangs
  - no other ways to disrupt parsing
- Strict and complete TOML support
  - fully supports TOML 1.0
  - supports draft of TOML 1.1
- Provide good integration with Qt, but avoid relying on untrusted Qt code.
  - does not rely on the UTF-8 parsing from Qt
  - does not rely on `QChar`
- Focus on a high-quality code base
  - modern C++ code
  - strict code style
  - well documented code

License
=======

Copyright © 2023-2026 Tobias Erbsland. Web: https://erbsland.dev/
Copyright © 2023-2026 EducateIT GmbH. Web: https://educateit.ch/

This program is free software: you can redistribute it and/or modify it under the terms of the
GNU Lesser General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program.
If not, see <https://www.gnu.org/licenses/>.

