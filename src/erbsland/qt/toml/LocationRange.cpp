// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "LocationRange.hpp"


namespace erbsland::qt::toml {


void LocationRange::extend(const LocationRange &other) noexcept {
    _begin = std::min(_begin, other._begin);
    _end = std::max(_end, other._end);
}


void LocationRange::extend(const Location &loc) noexcept {
    _begin = std::min(_begin, loc);
    _end = std::max(_end, loc);
}


auto LocationRange::toString(LocationFormat format) const noexcept -> QString {
    return _begin.toString(format) + " - " + _end.toString(format);
}


}


