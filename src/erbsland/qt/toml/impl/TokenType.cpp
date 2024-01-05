// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
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

