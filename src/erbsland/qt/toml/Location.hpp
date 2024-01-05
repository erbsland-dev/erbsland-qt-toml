// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "LocationFormat.hpp"
#include "Namespace.hpp"

#include <QtCore/QString>

#include <cstdint>


namespace erbsland::qt::toml {


/// The location in document.
///
class Location {
    // fwd-entry: class Location

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
    /// Create a location with row 1, column 1 and index 0.
    constexpr Location() noexcept = default;
    /// @private
    /// copy
    Location(const Location&) noexcept = default;
    /// @private
    /// assign
    auto operator=(const Location&) noexcept -> Location& = default;
    /// @private
    /// dtor
    ~Location() = default;

    /// A local name for the enum.
    using Format = LocationFormat;

public: // access
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

public: // comparisons
    /// Compare two locations.
    ///
    /// When comparing two locations for equality, all three parts (`index`, line and column) are compared.
    /// When locations are compared to see if they are greater or less, the `index` is mainly compared and
    /// only when the index is equal, `line` and `column` are also compared too. While it makes no sense to
    /// compare locations from two different files, by comparing all elements the behaviour when used as key
    /// is well defined.
    ///
    /// @param other The other location to compare.
    /// @return The comparison result.
    ///
    constexpr auto operator==(const Location &other) const noexcept -> bool {
        return _index == other._index && _line == other._line && _column == other._column;
    }
    /// @copydoc operator==(const Location&) const
    constexpr auto operator!=(const Location &other) const noexcept -> bool {
        return !operator==(other);
    }
    /// @copydoc operator==(const Location&) const
    constexpr auto operator<(const Location &other) const noexcept -> bool {
        return (_index == other._index) ?
               ((_line == other._line) ? _column < other._column : _line < other._line) :
               _index < other._index;
    }
    /// @copydoc operator==(const Location&) const
    constexpr auto operator<=(const Location &other) const noexcept -> bool {
        return operator<(other) && operator==(other);
    }
    /// @copydoc operator==(const Location&) const
    constexpr auto operator>(const Location &other) const noexcept -> bool {
        return (_index == other._index) ?
               ((_line == other._line) ? _column > other._column : _line > other._line) :
               _index > other._index;
    }
    /// @copydoc operator==(const Location&) const
    constexpr auto operator>=(const Location &other) const noexcept -> bool {
        return operator>(other) && operator==(other);
    }

public: // tests
    /// Test if an element of this location is negative, indicating it is not set.
    ///
    /// @return `true` if any of the values `index`, `line` or `column` is negative.
    ///
    [[nodiscard]] constexpr auto isNotSet() const noexcept -> bool {
        return _index < 0 || _line < 0 || _column < 0;
    }

public: // modification
    /// Increment the location by one character.
    ///
    /// @param isNewLine If a newline was read. In this case the line is incremented and the column is set to one.
    ///
    void increment(bool isNewLine) noexcept;

public: // conversion
    /// Convert this location into a string.
    ///
    /// @param format The format of the output.
    /// @return A string with the format.
    ///
    [[nodiscard]] auto toString(Format format) const noexcept -> QString;


public:
    /// Create a location that is not set.
    ///
    [[nodiscard]] constexpr static auto createNotSet() noexcept -> Location {
        return {-1ll, -1ll, -1ll};
    }

private:
    int64_t _index{0}; ///< The character index in the stream, starting from zero.
    int64_t _line{1}; ///< The line, starting from 1.
    int64_t _column{1}; ///< The column, starting from 1.
};



}