# Copyright © 2023 Tobias Erbsland. Web: https://erbsland.dev
# Copyright © 2023 EducateIT GmbH. Web: https://educateit.ch
#
# This program is free software: you can redistribute it and/or modify it under the terms of the
# GNU Lesser General Public License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along with this program.
# If not, see <https://www.gnu.org/licenses/>.

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
target_compile_features(erbsland-unittest PRIVATE cxx_std_17)
target_include_directories(erbsland-qt-toml PUBLIC ${QtCore_INCLUDE_DIRS})
target_link_libraries(erbsland-qt-toml PUBLIC Qt::Core)

