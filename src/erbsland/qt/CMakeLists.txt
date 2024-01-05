# Copyright © 2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch/
# According to the copyright terms specified in the file "COPYRIGHT.md".
# SPDX-License-Identifier: LGPL-3.0-or-later


cmake_minimum_required(VERSION 3.25)

add_subdirectory(toml)

target_sources(erbsland-qt-toml PRIVATE
        TomlParser.hpp
        TomlValue.hpp
)

