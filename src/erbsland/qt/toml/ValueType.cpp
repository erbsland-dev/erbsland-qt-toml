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
#include "ValueType.hpp"


namespace erbsland::qt::toml {


auto valueTypeToString(ValueType valueType) noexcept -> QString {
    switch (valueType) {
        case ValueType::Integer:
            return QStringLiteral("Integer");
        case ValueType::Float:
            return QStringLiteral("Float");
        case ValueType::Boolean:
            return QStringLiteral("Boolean");
        case ValueType::String:
            return QStringLiteral("String");
        case ValueType::Time:
            return QStringLiteral("Time");
        case ValueType::Date:
            return QStringLiteral("Date");
        case ValueType::DateTime:
            return QStringLiteral("DateTime");
        case ValueType::Table:
            return QStringLiteral("Table");
        case ValueType::Array:
            return QStringLiteral("Array");
        default:
            return QStringLiteral("Unknown");
    }
}


auto valueTypeToUnitTestString(ValueType valueType) noexcept -> QString {
    switch (valueType) {
        case ValueType::Integer:
            return QStringLiteral("integer");
        case ValueType::Float:
            return QStringLiteral("float");
        case ValueType::Boolean:
            return QStringLiteral("bool");
        case ValueType::String:
            return QStringLiteral("string");
        case ValueType::Time:
            return QStringLiteral("time-local");
        case ValueType::Date:
            return QStringLiteral("date-local");
        case ValueType::DateTime:
            return QStringLiteral("datetime");
        case ValueType::Table:
            return QStringLiteral("table");
        case ValueType::Array:
            return QStringLiteral("array");
        default:
            return QStringLiteral("unknown");
    }
}


}

