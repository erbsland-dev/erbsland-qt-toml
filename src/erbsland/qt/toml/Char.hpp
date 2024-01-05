// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
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
    /// Create a zero character.
    Char() noexcept = default;
    /// @private
    /// dtor
    ~Char() = default;
    /// @private
    /// copy
    Char(const Char&) noexcept= default;
    /// @private
    /// assign
    auto operator=(const Char&) noexcept -> Char& = default;

public: // Compare the char with itself.
    /// Compare this character with another.
    ///
    /// @param other The other character for the comparison.
    /// @return The result of the comparison.
    ///
    constexpr auto operator==(Char other) const noexcept -> bool {
        return _value == other._value;
    }
    /// @copydoc operator==(Char) const
    constexpr auto operator!=(Char other) const noexcept -> bool {
        return _value != other._value;
    }
    /// @copydoc operator==(Char) const
    constexpr auto operator>(Char other) const noexcept -> bool {
        return _value > other._value;
    }
    /// @copydoc operator==(Char) const
    constexpr auto operator>=(Char other) const noexcept -> bool {
        return _value >= other._value;
    }
    /// @copydoc operator==(Char) const
    constexpr auto operator<(Char other) const noexcept -> bool {
        return _value < other._value;
    }
    /// @copydoc operator==(Char) const
    constexpr auto operator<=(Char other) const noexcept -> bool {
        return _value <= other._value;
    }

public: // Compare the unicode character with an ascii character.
    /// Compare this character with an ascii character.
    ///
    /// @param asciiChar The ascii character.
    /// @return The result of the comparison.
    ///
    constexpr auto operator==(char asciiChar) const noexcept -> bool {
        return _value == static_cast<char32_t>(asciiChar);
    }
    /// @copydoc operator==(char) const
    constexpr auto operator!=(char asciiChar) const noexcept -> bool {
        return _value != static_cast<char32_t>(asciiChar);
    }
    /// @copydoc operator==(char) const
    constexpr auto operator<(char asciiChar) const noexcept -> bool {
        return _value < static_cast<char32_t>(asciiChar);
    }
    /// @copydoc operator==(char) const
    constexpr auto operator<=(char asciiChar) const noexcept -> bool {
        return _value <= static_cast<char32_t>(asciiChar);
    }
    /// @copydoc operator==(char) const
    constexpr auto operator>(char asciiChar) const noexcept -> bool {
        return _value > static_cast<char32_t>(asciiChar);
    }
    /// @copydoc operator==(char) const
    constexpr auto operator>=(char asciiChar) const noexcept -> bool {
        return _value >= static_cast<char32_t>(asciiChar);
    }

public: // Safely compare the unicode character with any other integer like type.
    /// Compare this characters code point with an integer value.
    ///
    /// @tparam Int The integer type.
    /// @param anyInteger The integer for the comparison.
    /// @return The result of the comparison.
    ///
    template<typename Int, std::enable_if_t<std::is_integral_v<Int>, bool> = true>
    constexpr auto operator==(Int anyInteger) const noexcept -> bool {
        if constexpr (std::is_signed_v<Int>) {
            return anyInteger >= 0 && _value == static_cast<char32_t>(anyInteger);
        } else {
            return _value == static_cast<char32_t>(anyInteger);
        }
    }
    /// @copydoc operator==(Int) const
    template<typename Int, std::enable_if_t<std::is_integral_v<Int>, bool> = true>
    constexpr auto operator!=(Int anyInteger) const noexcept -> bool {
        return !operator==(anyInteger);
    }
    /// @copydoc operator==(Int) const
    template<typename Int, std::enable_if_t<std::is_integral_v<Int>, bool> = true>
    constexpr auto operator>(Int anyInteger) const noexcept -> bool {
        if constexpr (std::is_signed_v<Int>) {
            return anyInteger < 0 || _value > static_cast<char32_t>(anyInteger);
        } else {
            return _value > static_cast<char32_t>(anyInteger);
        }
    }
    /// @copydoc operator==(Int) const
    template<typename Int, std::enable_if_t<std::is_integral_v<Int>, bool> = true>
    constexpr auto operator>=(Int anyInteger) const noexcept -> bool {
        return operator==(anyInteger) || operator>(anyInteger);
    }
    /// @copydoc operator==(Int) const
    template<typename Int, std::enable_if_t<std::is_integral_v<Int>, bool> = true>
    constexpr auto operator<(Int anyInteger) const noexcept -> bool {
        if constexpr (std::is_signed_v<Int>) {
            return anyInteger >= 0 && _value < static_cast<char32_t>(anyInteger);
        } else {
            return _value < static_cast<uint32_t>(anyInteger);
        }
    }
    /// @copydoc operator==(Int) const
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

