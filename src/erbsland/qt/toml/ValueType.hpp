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


#include "Namespace.hpp"

#include <QtCore/QString>


namespace erbsland::qt::toml {


/// The type of a value.
///
enum class ValueType {
    Integer, ///< A signed integer.
    Float, ///< A floating-point number.
    Boolean, ///< A boolean value (either `true` or `false`).
    String, ///< A string.
    Time, ///< A local time of day.
    Date, ///< A local calendar date.
    DateTime, ///< A date and time with or without offset.
    Table, ///< An unordered map of TOML key-value pairs.
    Array, ///< An ordered list of TOML values.
};


/// Convert a value type enumeration into a string.
///
/// @param valueType The value source.
/// @return A string for the enum value, like `Integer`.
///
auto valueTypeToString(ValueType valueType) noexcept -> QString;


/// Convert a value type enumeration into a string.
///
/// This function is like `valueTypeToString`, but returns the type information as: `integer`,
/// `float`, `bool`, `string`, `date-local`, `time-local`, `datetime`, `table`, `array`. There is no `datetime-local`,
/// as you have to inspect the actual date/time value to determine if its a local or offset date.
///
/// @param valueType The value source.
/// @return A string for the enum value, like `integer`.
///
auto valueTypeToUnitTestString(ValueType valueType) noexcept -> QString;


}
