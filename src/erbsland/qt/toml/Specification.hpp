// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "Namespace.hpp"

#include <QtCore/QString>


namespace erbsland::qt::toml {


/// The specification version.
///
enum class Specification : int {
    Version_1_0 = 0,
    Version_1_1
};


/// Convert a value source enumeration into a string.
///
/// @param specification The specification.
/// @return A string for the specification in the format `TOML 1.0`.
///
auto specificationToString(Specification specification) noexcept -> QString;


}

