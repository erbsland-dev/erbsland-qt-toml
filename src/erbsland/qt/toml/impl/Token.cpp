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

