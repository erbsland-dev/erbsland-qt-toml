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

