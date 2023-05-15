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


#include "Location.hpp"

#include <algorithm>


namespace erbsland::qt::toml {


/// The range covered by two locations in a file.
///
class LocationRange {
    // fwd-entry: class LocationRange

public:
    /// Create a new location range.
    ///
    /// @param begin The beginning location.
    /// @param end The ending location.
    ///
    constexpr LocationRange(const Location& begin, const Location& end) noexcept
        : _begin{begin}, _end{end} {
    }

    // defaults
    constexpr LocationRange() noexcept = default;
    LocationRange(const LocationRange&) noexcept = default;
    auto operator=(const LocationRange&) noexcept -> LocationRange& = default;

public: // access
    /// Get the beginning of the location range.
    ///
    /// @return The beginning location.
    ///
    [[nodiscard]] constexpr auto begin() const noexcept -> const Location& {
        return _begin;
    }

    /// Get the end of the location range.
    ///
    /// @return The ending location.
    ///
    [[nodiscard]] constexpr auto end() const noexcept -> const Location& {
        return _end;
    }

public: // comparisons
    constexpr auto operator==(const LocationRange& other) const noexcept -> bool {
        return _begin == other._begin && _end == other._end;
    }

    constexpr auto operator!=(const LocationRange& other) const noexcept -> bool {
        return !operator==(other);
    }

public: // modification
    /// Extend the current location range.
    ///
    /// @param other The other location range to extend with.
    ///
    void extend(const LocationRange& other) noexcept;

    /// Extend the current location range.
    ///
    /// @param loc The location to extend with.
    ///
    void extend(const Location& loc) noexcept;

public: // conversion
    /// Convert this location range into a string.
    ///
    /// @param format The format of the output.
    /// @return A string with the format.
    ///
    [[nodiscard]] auto toString(LocationFormat format) const noexcept -> QString;

public:
    /// Create a location range that is not set.
    ///
    [[nodiscard]] constexpr static auto createNotSet() noexcept -> LocationRange {
        return LocationRange{Location::createNotSet(), Location::createNotSet()};
    }

private:
    Location _begin; ///< The begin location.
    Location _end; ///< The end location.
};


}

