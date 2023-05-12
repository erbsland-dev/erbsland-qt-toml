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

#include <QtCore/QString>

#include <cstdint>


namespace erbsland::qt::toml {


/// A 32bit unicode character.
///
class Char {
public:
    /// Create a new char from its unicode code point.
    ///
    /// @param value The unicode value.
    ///
    constexpr explicit Char(char32_t value) noexcept : _value{value} {}

    // defaults
    Char() noexcept = default; // zero character.
    ~Char() = default;
    Char(const Char&) noexcept= default;
    auto operator=(const Char&) noexcept -> Char& = default;

public: // Compare the char with itself.
    constexpr auto operator==(Char other) const noexcept -> bool {
        return _value == other._value;
    }
    constexpr auto operator!=(Char other) const noexcept -> bool {
        return _value != other._value;
    }
    constexpr auto operator>(Char other) const noexcept -> bool {
        return _value > other._value;
    }
    constexpr auto operator>=(Char other) const noexcept -> bool {
        return _value >= other._value;
    }
    constexpr auto operator<(Char other) const noexcept -> bool {
        return _value < other._value;
    }
    constexpr auto operator<=(Char other) const noexcept -> bool {
        return _value <= other._value;
    }

public: // Compare the unicode character with an ascii character.
    constexpr auto operator==(char asciiChar) const noexcept -> bool {
        return _value == static_cast<char32_t>(asciiChar);
    }
    constexpr auto operator!=(char asciiChar) const noexcept -> bool {
        return _value != static_cast<char32_t>(asciiChar);
    }
    constexpr auto operator<(char asciiChar) const noexcept -> bool {
        return _value < static_cast<char32_t>(asciiChar);
    }
    constexpr auto operator<=(char asciiChar) const noexcept -> bool {
        return _value <= static_cast<char32_t>(asciiChar);
    }
    constexpr auto operator>(char asciiChar) const noexcept -> bool {
        return _value > static_cast<char32_t>(asciiChar);
    }
    constexpr auto operator>=(char asciiChar) const noexcept -> bool {
        return _value >= static_cast<char32_t>(asciiChar);
    }

public: // Safely compare the unicode character with any other integer like type.
    template<typename Int, std::enable_if_t<std::is_integral_v<Int>, bool> = true>
    constexpr auto operator==(Int anyInteger) const noexcept -> bool {
        if constexpr (std::is_signed_v<Int>) {
            return anyInteger >= 0 && _value == static_cast<char32_t>(anyInteger);
        } else {
            return _value == static_cast<char32_t>(anyInteger);
        }
    }
    template<typename Int, std::enable_if_t<std::is_integral_v<Int>, bool> = true>
    constexpr auto operator!=(Int anyInteger) const noexcept -> bool {
        return !operator==(anyInteger);
    }
    template<typename Int, std::enable_if_t<std::is_integral_v<Int>, bool> = true>
    constexpr auto operator>(Int anyInteger) const noexcept -> bool {
        if constexpr (std::is_signed_v<Int>) {
            return anyInteger < 0 || _value > static_cast<char32_t>(anyInteger);
        } else {
            return _value > static_cast<char32_t>(anyInteger);
        }
    }
    template<typename Int, std::enable_if_t<std::is_integral_v<Int>, bool> = true>
    constexpr auto operator>=(Int anyInteger) const noexcept -> bool {
        return operator==(anyInteger) || operator>(anyInteger);
    }
    template<typename Int, std::enable_if_t<std::is_integral_v<Int>, bool> = true>
    constexpr auto operator<(Int anyInteger) const noexcept -> bool {
        if constexpr (std::is_signed_v<Int>) {
            return anyInteger >= 0 && _value < static_cast<char32_t>(anyInteger);
        } else {
            return _value < static_cast<uint32_t>(anyInteger);
        }
    }
    template<typename Int, std::enable_if_t<std::is_integral_v<Int>, bool> = true>
    constexpr auto operator<=(Int anyInteger) const noexcept -> bool {
        return operator==(anyInteger) || operator<(anyInteger);
    }

public: // tests
    /// Test if this is a null character.
    ///
    [[nodiscard]] constexpr auto isNull() const noexcept -> bool {
        return _value == 0U;
    }

    /// Test if this character is a valid unicode character.
    ///
    [[nodiscard]] constexpr auto isValidUnicode() const noexcept -> bool {
        return (_value < 0xd800U || _value > 0xdfffU) && _value <= 0x10ffffU;
    }

public: // conversion
    /// If possible, covert the unicode character to an ascii character.
    ///
    /// @return The ascii character, or if the unicode character is >0x7f, zero is returned.
    ///
    [[nodiscard]] constexpr auto toAscii() const noexcept -> char {
        return (_value > 0x7fU) ? '\0' : static_cast<char>(_value);
    }

    /// Append this character to a QString.
    ///
    /// @param str The string to append this character.
    ///
    inline void appendToString(QString &str) const noexcept {
        if (_value >= 0x10000U) {
            str.append(QString::fromUcs4(&_value, 1));
        } else {
            str.append(QChar{static_cast<uint16_t>(_value)});
        }
    }

private:
    char32_t _value{}; ///< The value of this unicode character.
};


}

