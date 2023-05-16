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

