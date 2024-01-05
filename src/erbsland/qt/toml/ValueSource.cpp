// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "ValueSource.hpp"


namespace erbsland::qt::toml {


auto valueSourceToString(ValueSource valueSource) noexcept -> QString {
    switch (valueSource) {
        case ValueSource::ImplicitTable:
            return QStringLiteral("Implicit Table");
        case ValueSource::ExplicitTable:
            return QStringLiteral("Explicit Table");
        case ValueSource::ImplicitValue:
            return QStringLiteral("Implicit Value");
        case ValueSource::ExplicitValue:
            return QStringLiteral("Explicit Value");
        case ValueSource::Value:
            return QStringLiteral("Value");
        default:
            return QStringLiteral("Unknown");
    }
}


}

