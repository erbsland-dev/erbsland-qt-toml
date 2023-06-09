// Copyright © 2023 Tobias Erbsland. Web: https://erbsland.dev
// Copyright © 2023 EducateIT GmbH. Web: https://educateit.ch
//
// This program is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with this program.
// If not, see <https://www.gnu.org/licenses/>.
#pragma once


namespace erbsland::qt::toml {


/// The string format for the location.
///
enum class LocationFormat {
    Compact, ///< Generates a text in the format `[line]:[column]`.
    CompactWithIndex, ///< Generates a text in the format `[line]:[column]:([index])`.
    Long, ///< Generates a text in the format `line [line], column [column]`.
    LongWithIndex, ///< Generates a text in the format `line [line], column [column] (index [index])`.
};


}

