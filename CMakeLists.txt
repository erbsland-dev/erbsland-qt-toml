# Copyright © 2024-2026 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch/
# According to the copyright terms specified in the file "COPYRIGHT.md".
# SPDX-License-Identifier: LGPL-3.0-or-later

cmake_minimum_required(VERSION 3.25)

project(erbsland-qt-toml
        VERSION 1.3.0
        DESCRIPTION "Erbsland Qt TOML Library"
        HOMEPAGE_URL https://erbsland.dev/
        LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(erbsland-qt-toml STATIC)
add_subdirectory(src/erbsland/qt)
target_compile_features(erbsland-qt-toml PUBLIC cxx_std_17)
target_include_directories(erbsland-qt-toml INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
target_link_libraries(erbsland-qt-toml PUBLIC Qt6::Core)

