// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
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
