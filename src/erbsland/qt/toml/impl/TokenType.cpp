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
#include "TokenType.hpp"


namespace erbsland::qt::toml::impl {


auto tokenTypeToString(TokenType tokenType) noexcept -> QString {
    switch (tokenType) {
    case TokenType::Whitespace: return QStringLiteral("Whitespace");
    case TokenType::NewLine: return QStringLiteral("NewLine");
    case TokenType::Comment: return QStringLiteral("Comment");
    case TokenType::EndOfDocument: return QStringLiteral("EndOfDocument");
    case TokenType::BareKey: return QStringLiteral("BareKey");
    case TokenType::NameSeperator: return QStringLiteral("NameSeperator");
    case TokenType::Assignment: return QStringLiteral("Assignment");
    case TokenType::TableNameBegin: return QStringLiteral("TableNameBegin");
    case TokenType::TableNameEnd: return QStringLiteral("TableNameEnd");
    case TokenType::ArrayNameBegin: return QStringLiteral("ArrayNameBegin");
    case TokenType::ArrayNameEnd: return QStringLiteral("ArrayNameEnd");
    case TokenType::SingleLineString: return QStringLiteral("SingleLineString");
    case TokenType::MultiLineString: return QStringLiteral("MultiLineString");
    case TokenType::DecimalInteger: return QStringLiteral("DecimalInteger");
    case TokenType::HexInteger: return QStringLiteral("HexInteger");
    case TokenType::BinaryInteger: return QStringLiteral("BinaryInteger");
    case TokenType::OctalInteger: return QStringLiteral("OctalInteger");
    case TokenType::Float: return QStringLiteral("Float");
    case TokenType::Boolean: return QStringLiteral("Boolean");
    case TokenType::OffsetDateTime: return QStringLiteral("OffsetDateTime");
    case TokenType::LocalDateTime: return QStringLiteral("LocalDateTime");
    case TokenType::LocalDate: return QStringLiteral("LocalDate");
    case TokenType::LocalTime: return QStringLiteral("LocalTime");
    case TokenType::TableBegin: return QStringLiteral("TableBegin");
    case TokenType::TableSeperator: return QStringLiteral("TableSeperator");
    case TokenType::TableEnd: return QStringLiteral("TableEnd");
    case TokenType::ArrayBegin: return QStringLiteral("ArrayBegin");
    case TokenType::ArrayEnd: return QStringLiteral("ArrayEnd");
    default: return {};
    }
}


}

