// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include <QtCore/QString>


namespace erbsland::qt::toml::impl {


/// @private
/// The type of a token.
///
enum class TokenType {
    // tokens in all contexts:
    Whitespace, ///< A block of whitespace.
    NewLine, ///< A line break outside of a string.
    Comment, ///< A comment
    EndOfDocument, ///< Reached the end of the document.
    BareKey, ///< A name. String or bare key. In value context only the bare keys.
    NameSeperator, ///< `.`
    Assignment, ///< `=`
    TableNameBegin, ///< `[`
    TableNameEnd, ///< ']'
    ArrayNameBegin, ///< `[[`
    ArrayNameEnd, ///< `]]`
    // value structure
    TableBegin, ///< `{`
    TableSeperator, ///< ','
    TableEnd, ///< `}`
    ArrayBegin, ///< `[`
    ArrayEnd, ///< `]`
    // values
    SingleLineString, ///< Single line string.
    MultiLineString, ///< A multi line string.
    DecimalInteger, ///< An decimal integer value.
    HexInteger, ///< A hexadecimal integer value.
    BinaryInteger, ///< A binary integer value.
    OctalInteger, ///< An octal integer value.
    Float, ///< A float value.
    Boolean, ///< A boolean identifier.
    OffsetDateTime, ///< A date/time value with an offset.
    LocalDateTime, ///< A date/time without offset.
    LocalDate, ///< A date
    LocalTime, ///< A time
};


auto tokenTypeToString(TokenType tokenType) noexcept -> QString;


}

