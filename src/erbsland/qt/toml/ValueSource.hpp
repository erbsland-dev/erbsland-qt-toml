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


/// The source that defined the value.
///
enum class ValueSource {
    ImplicitTable, ///< Implicit key of a table `[this.this.key]`
    ExplicitTable, ///< Explicit key of a table `[key.key.this]`
    ImplicitValue, ///< Implicit key of a value `this.key.name = 5`
    ExplicitValue, ///< Explicit key of a value `key.this.name = 5`
    Value, ///< A value or an inline table or list: `key = { ... }` or `key = [ ... ]`
};


}

