# Copyright © 2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch/
# According to the copyright terms specified in the file "COPYRIGHT.md".
# SPDX-License-Identifier: LGPL-3.0-or-later

cmake_minimum_required(VERSION 3.25)

project(erbsland-qt-toml
        DESCRIPTION "Erbsland Qt TOML Library"
        HOMEPAGE_URL https://erbsland.dev/
        LANGUAGES CXX)

if (NOT DEFINED Qt5Core_FOUND AND NOT DEFINED Qt6Core_FOUND)
    message(WARNING "No global Qt configuration found.")
    find_package(Qt5 REQUIRED COMPONENTS Core)
endif ()

add_library(erbsland-qt-toml STATIC)
add_subdirectory(src/erbsland/qt)
set_property(TARGET erbsland-qt-toml PROPERTY CXX_STANDARD 17)
target_compile_features(erbsland-qt-toml PRIVATE cxx_std_17)
target_include_directories(erbsland-qt-toml PUBLIC ${QtCore_INCLUDE_DIRS})
target_include_directories(erbsland-qt-toml INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
target_link_libraries(erbsland-qt-toml PUBLIC Qt::Core)

