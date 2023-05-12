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

#include <cstdint>


namespace erbsland::qt::toml {


/// The location in document.
///
class Location {
public:
    /// Create a new location.
    ///
    /// @param index The character index in the stream, starts from zero.
    /// @param line The line number in the document, starts from one.
    /// @param column The column number in the document, starts from one.
    ///
    constexpr Location(int64_t index, int64_t line, int64_t column) noexcept
        : _index{index}, _line{line}, _column{column} {
    }

    // defaults
    constexpr Location() noexcept = default;
    Location(const Location&) noexcept = default;
    auto operator=(const Location&) noexcept -> Location& = default;
    ~Location() = default;

public:
    /// Get the index of the location.
    ///
    /// @note Do not confuse this with the byte index.
    /// @return The character index in the current document, starting from zero.
    ///
    [[nodiscard]] constexpr auto index() const noexcept -> int64_t { return _index; }

    /// Get the line of the location.
    ///
    /// @return The line, starting from one.
    ///
    [[nodiscard]] constexpr auto line() const noexcept -> int64_t { return _line; }

    /// Get the column of the location.
    ///
    /// @return The column, starting from one.
    ///
    [[nodiscard]] constexpr auto column() const noexcept -> int64_t { return _column; }

    /// Test if this is a negative location.
    ///
    /// Negative values can be used to indicate no existing location.
    ///
    /// @return `true` if any of the values `index`, `line` or `column` is negative.
    ///
    [[nodiscard]] constexpr auto isNegative() const noexcept -> bool {
        return _index < 0 || _line < 0 || _column < 0;
    }

    /// Increment the location by one character.
    ///
    /// @param isNewLine If a newline was read. In this case the line is incremented and the column is set to one.
    ///
    inline void increment(bool isNewLine) noexcept {
        _index += 1;
        if (isNewLine) {
            _column = 1;
            _line += 1;
        } else {
            _column += 1;
        }
    }

private:
    int64_t _index{0}; ///< The character index in the stream, starting from zero.
    int64_t _line{1}; ///< The line, starting from 1.
    int64_t _column{1}; ///< The column, starting from 1.
};



}