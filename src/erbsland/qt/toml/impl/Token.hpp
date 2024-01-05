// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "TokenType.hpp"

#include "../LocationRange.hpp"

#include <QtCore/QString>

#include <utility>


namespace erbsland::qt::toml::impl {


/// @private
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
    /// @param range The location range.
    ///
    inline Token(TokenType type, QString text, LocationRange range) noexcept
        : _type{type}, _text{std::move(text)}, _range{range} {
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
        return _range.begin();
    }

    /// Get the end location.
    ///
    [[nodiscard]] inline auto end() const noexcept -> Location {
        return _range.end();
    }

    /// Get the location range.
    ///
    [[nodiscard]] inline auto range() const noexcept -> LocationRange {
        return _range;
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
    LocationRange _range{LocationRange::createNotSet()}; ///< The location range.
};


}

