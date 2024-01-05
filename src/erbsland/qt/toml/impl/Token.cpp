// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "Token.hpp"


#include <QtCore/QRegularExpression>


namespace erbsland::qt::toml::impl {


auto Token::toDebugString() const noexcept -> QString {
    if (_text.isEmpty()) {
        return tokenTypeToString(_type);
    }
    static auto reSpecialChars = QRegularExpression(R"([\p{C}]+)");
    auto text = _text;
    text.replace(reSpecialChars, " "); // in erbsland core, encode special characters.
    return QStringLiteral("%1(\"%2\")").arg(tokenTypeToString(_type), _text);
}


auto Token::isValue() const noexcept -> bool {
    switch (_type) {
    case TokenType::SingleLineString:
    case TokenType::MultiLineString:
    case TokenType::DecimalInteger:
    case TokenType::HexInteger:
    case TokenType::BinaryInteger:
    case TokenType::OctalInteger:
    case TokenType::Float:
    case TokenType::Boolean:
    case TokenType::OffsetDateTime:
    case TokenType::LocalDateTime:
    case TokenType::LocalDate:
    case TokenType::LocalTime:
        return true;
    default:
        return false;
    }
}


auto Token::isKey() const noexcept -> bool {
    if (_type == TokenType::DecimalInteger && _text.at(0) != '+') {
        return true; // bare keys can look like decimal integers.
    }
    return _type == TokenType::BareKey || _type == TokenType::SingleLineString;
}


}

