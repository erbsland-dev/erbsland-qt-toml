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


#include <QtCore/QString>


namespace erbsland::qt::toml::impl {


/// @internal
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

