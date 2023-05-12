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


#include "TokenType.hpp"

#include "../Location.hpp"

#include <QtCore/QString>

#include <utility>


namespace erbsland::qt::toml::impl {


/// @internal
/// Represents a token read from the tokenizer.
///
class Token final {
public:
    /// Create a token.
    ///
    /// @param type The token type.
    /// @param text The token text.
    ///
    inline Token(TokenType type, QString text) noexcept
        : _type{type}, _text{std::move(text)} {
    }

    /// Create a token.
    ///
    /// @param type The token type.
    /// @param text The token text.
    /// @param begin The begin location.
    /// @param end The end location.
    ///
    inline Token(TokenType type, QString text, Location begin, Location end) noexcept
        : _type{type}, _text{std::move(text)}, _begin{begin}, _end{end} {
    }

    // defaults
    Token() noexcept = default;
    Token(const Token&) noexcept = default;
    auto operator=(const Token&) noexcept -> Token& = default;

public:
    constexpr auto operator==(const Token &other) noexcept -> bool {
        return _type == other._type && _text == other._text;
    }
    constexpr auto operator!=(const Token &other) noexcept -> bool {
        return !operator==(other);
    }

public:
    /// Get the token type.
    ///
    [[nodiscard]] inline auto type() const noexcept -> TokenType {
        return _type;
    }

    /// Get the token text.
    ///
    [[nodiscard]] inline auto text() const noexcept -> const QString& {
        return _text;
    }

    /// Get the begin location.
    ///
    [[nodiscard]] inline auto begin() const noexcept -> Location {
        return _begin;
    }

    /// Get the end location.
    ///
    [[nodiscard]] inline auto end() const noexcept -> Location {
        return _end;
    }

    /// Test if this token is a value.
    ///
    [[nodiscard]] auto isValue() const noexcept -> bool;

    /// Test if this token is a name (BareKey or SingleLineString).
    ///
    [[nodiscard]] auto isKey() const noexcept -> bool;

    /// Test if this token is a name separator
    ///
    [[nodiscard]] inline auto isKeySeperator() const noexcept -> bool {
        return _type == TokenType::NameSeperator;
    }

    /// Test if this token is the end of the document.
    ///
    [[nodiscard]] inline auto isEndOfDocument() const noexcept -> bool {
        return _type == TokenType::EndOfDocument;
    }

    /// Test if this token is whitespace (space, tab or comment).
    ///
    [[nodiscard]] inline auto isWhiteSpace() const noexcept -> bool {
        return _type == TokenType::Whitespace || _type == TokenType::Comment;
    }

    /// Test if this token is a newline.
    ///
    [[nodiscard]] inline auto isNewLine() const noexcept -> bool {
        return _type == TokenType::NewLine;
    }

    /// Create a string for debugging.
    ///
    [[nodiscard]] auto toDebugString() const noexcept -> QString;

private:
    TokenType _type{TokenType::EndOfDocument}; ///< The type of the token.
    QString _text{}; ///< The text of the token.
    Location _begin{}; ///< The begin location.
    Location _end{}; ///< The end location.
};


}

