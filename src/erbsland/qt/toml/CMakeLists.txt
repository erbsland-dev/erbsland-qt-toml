# Copyright © 2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch/
# According to the copyright terms specified in the file "COPYRIGHT.md".
# SPDX-License-Identifier: LGPL-3.0-or-later


cmake_minimum_required(VERSION 3.25)

add_subdirectory(impl)

target_sources(erbsland-qt-toml PRIVATE
        Char.hpp
        Error.cpp
        Error.hpp
        InputStream.cpp
        InputStream.hpp
        Location.cpp
        Location.hpp
        LocationFormat.hpp
        LocationRange.cpp
        LocationRange.hpp
        Namespace.hpp
        Parser.cpp
        Parser.hpp
        Specification.cpp
        Specification.hpp
        Value.cpp
        Value.hpp
        ValueIterator.cpp
        ValueIterator.hpp
        ValueSource.cpp
        ValueSource.hpp
        ValueType.cpp
        ValueType.hpp
)

