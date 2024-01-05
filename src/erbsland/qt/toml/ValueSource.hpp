// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "Namespace.hpp"

#include <QtCore/QString>


namespace erbsland::qt::toml {


/// The source that defined the value.
///
enum class ValueSource {
    ImplicitTable, ///< Implicit key of a table `[this.this.key]`
    ExplicitTable, ///< Explicit key of a table `[key.key.this]`
    ImplicitValue, ///< Implicit key of a value `this.key.name = 5`
    ExplicitValue, ///< Explicit key of a value `key.this.name = 5`
    Value, ///< A value or an inline table or list: `key = { ... }` or `key = [ ... ]`
};


/// Convert a value source enumeration into a string.
///
/// @param valueSource The value source.
/// @return A string for the enum value, like `Implicit Table`.
///
auto valueSourceToString(ValueSource valueSource) noexcept -> QString;


}

