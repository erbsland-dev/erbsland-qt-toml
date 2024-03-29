// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
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
    /// Create a location range 1:1(0)-1:1(0).
    constexpr LocationRange() noexcept = default;
    /// @private
    /// copy
    LocationRange(const LocationRange&) noexcept = default;
    /// @private
    /// assign
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
    /// Compare two location ranges.
    ///
    /// @param other The other location range for comparison.
    /// @return The result of the comparison.
    ///
    constexpr auto operator==(const LocationRange& other) const noexcept -> bool {
        return _begin == other._begin && _end == other._end;
    }
    /// @copydoc operator==(const LocationRange&) const
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

