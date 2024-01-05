// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "Location.hpp"


namespace erbsland::qt::toml {


auto Location::toString(Location::Format format) const noexcept -> QString {
    switch (format) {
        case LocationFormat::Compact:
        default:
            if (isNotSet()) {
                return QStringLiteral("?:?");
            }
            return QStringLiteral("%1:%2").arg(_line).arg(_column);
        case LocationFormat::CompactWithIndex:
            if (isNotSet()) {
                return QStringLiteral("?:?(?)");
            }
            return QStringLiteral("%1:%2(%3)").arg(_line).arg(_column).arg(_index);
        case LocationFormat::Long:
            if (isNotSet()) {
                return QStringLiteral("unknown location");
            }
            return QStringLiteral("line %1, column %2").arg(_line).arg(_column);
        case LocationFormat::LongWithIndex:
            if (isNotSet()) {
                return QStringLiteral("unknown location");
            }
            return QStringLiteral("line %1, column %2 (index %3)").arg(_line).arg(_column).arg(_index);
    }
}


void Location::increment(bool isNewLine) noexcept {
    _index += 1;
    if (isNewLine) {
        _column = 1;
        _line += 1;
    } else {
        _column += 1;
    }
}


}

