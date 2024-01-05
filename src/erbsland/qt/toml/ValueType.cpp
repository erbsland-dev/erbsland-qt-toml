// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
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

