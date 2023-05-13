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
#include "Tokenizer.hpp"


#include "../Error.hpp"


namespace erbsland::qt::toml::impl {


const std::vector<const char*> Tokenizer::_floatSpecials = {
    "inf",
    "nan",
    "+inf",
    "+nan",
    "-inf",
    "-nan"
};


const std::vector<const char*> Tokenizer::_booleanValues = {
    "true",
    "false"
};


Tokenizer::Tokenizer(Specification specification) noexcept
    : _specification{specification}, _reader{specification} {
}


void Tokenizer::startWithStream(const InputStreamPtr &inputStream) noexcept {
    _reader.resetWithInputStream(inputStream);
    _tokenContext = TokenContext::Structure;
    _valueNestingCount = 0;
    _stringQuotes = StringQuotes::None;
    _stringMode = StringMode::None;
}


void Tokenizer::stop() {
    _reader.resetWithInputStream({});
}


auto Tokenizer::createToken(TokenType tokenType) -> Token {
    auto [buffer, range] = _reader.takeToken();
    auto token = Token(tokenType, buffer, range);
    _stringQuotes = StringQuotes::None;
    _stringMode = StringMode::None;
    return token;
}


auto Tokenizer::read() -> Token {
    if (_reader.atEnd()) {
        return createToken(TokenType::EndOfDocument);
    }
    _reader.readNextChar();
    // Test for the token that can be everywhere.
    if (_reader.isWhiteSpace()) {
        return readWhiteSpace();
    }
    if (_reader.isComment()) {
        return readComment();
    }
    if (_reader.isNewLineOrCarriageReturn()) {
        _reader.skipNewLine(); // does not matter if this is the end of the file.
        return createToken(TokenType::NewLine);
    }
    // Test for structure or value tokens.
    if (_tokenContext == TokenContext::Structure) {
        auto token = readStructure();
        if (token.type() == TokenType::Assignment) {
            _tokenContext = TokenContext::Value;
        }
        return token;
    }
    // Here, we implement a basic switch between structure and values.
    // As this is only a tokenizer, there is no check if e.g. nested arrays and tables make sense,
    // that is the responsibility of the calling parser.
    auto token = readValue();
    if (token.type() == TokenType::ArrayBegin || token.type() == TokenType::TableBegin) {
        _valueNestingCount += 1;
        if (_valueNestingCount > cValueNestingLimit) {
            _reader.throwSyntaxError(QStringLiteral("Maximum number of nested structures exceed."));
        }
    } else if (token.type() == TokenType::ArrayEnd || token.type() == TokenType::TableEnd) {
        // Wait until all nested tables and arrays end, until switching back to structure context.
        _valueNestingCount -= 1;
        if (_valueNestingCount == 0) {
            _tokenContext = TokenContext::Structure;
        }
    } else if (_valueNestingCount == 0 && token.isValue()) {
        // If there is no nesting, switch back to structure after reading
        _tokenContext = TokenContext::Structure;
    }
    return token;
}


auto Tokenizer::readWhiteSpace() -> Token {
    _reader.skipWhiteSpace(); // whitespace may end anywhere.
    return createToken(TokenType::Whitespace);
}


auto Tokenizer::readComment() -> Token {
    if (_reader.skipCharAndTestAtEnd()) {
        return createToken(TokenType::Comment); // # at the end of the stream is a valid comment.
    }
    while (!_reader.isNewLineOrCarriageReturn()) {
        if (_reader.isControlCharacter()) {
            _reader.throwSyntaxError(QStringLiteral("Control characters are not allowed in comments."));
        }
        if (_reader.skipCharAndTestAtEnd()) {
            break; // # comments that ends with the stream is valid.
        }
    }
    return createToken(TokenType::Comment);
}


auto Tokenizer::readStructure() -> Token {
    if (_reader.isAssignment()) {
        _reader.skipCharAndExpectMore();
        return createToken(TokenType::Assignment);
    }
    if (_reader.isDot()) {
        _reader.skipCharAndExpectMore();
        return createToken(TokenType::NameSeperator);
    }
    if (isString()) {
        return readString();
    }
    if (_reader.isBareKey()) {
        return readBareKey();
    }
    if (_reader.isArrayBegin()) {
        _reader.skipCharAndExpectMore();
        if (_reader.isArrayBegin()) {
            _reader.skipCharAndExpectMore();
            return createToken(TokenType::ArrayNameBegin);
        }
        return createToken(TokenType::TableNameBegin);
    }
    if (_reader.isArrayEnd()) {
        _reader.skipCharAndExpectMore();
        if (_reader.isArrayEnd()) {
            _reader.skipCharAndExpectMore();
            return createToken(TokenType::ArrayNameEnd);
        }
        return createToken(TokenType::TableNameEnd);
    }
    _reader.throwUnexpectedCharacter();
}


auto Tokenizer::readValue() -> Token {
    if (_reader.isAssignment()) {
        _reader.skipCharAndExpectMore();
        return createToken(TokenType::Assignment);
    }
    if (_reader.isDot()) {
        _reader.skipCharAndExpectMore();
        return createToken(TokenType::NameSeperator);
    }
    if (isString()) {
        return readString();
    }
    if (_reader.isArrayBegin()) {
        _reader.skipCharAndExpectMore();
        return createToken(TokenType::ArrayBegin);
    }
    if (_reader.isArrayEnd()) {
        _reader.skipChar(); // may be at the end of the file.
        return createToken(TokenType::ArrayEnd);
    }
    if (_reader.isTableBegin()) {
        _reader.skipCharAndExpectMore();
        return createToken(TokenType::TableBegin);
    }
    if (_reader.isTableEnd()) {
        _reader.skipChar(); // may be at the end of the file.
        return createToken(TokenType::TableEnd);
    }
    if (_reader.isComma()) {
        _reader.skipCharAndExpectMore();
        return createToken(TokenType::TableSeperator);
    }
    readOptionalPlusMinusSign();
    if (_reader.isDecimalDigit()) {
        // That can also be a float, date or time.
        return readNumberLike();
    }
    // Every other text that starts with the allowed characters for a bare key is considered as one.
    if (_reader.isBareKey()) {
        return readFloatBoolOrBareKey();
    }
    if (_readSign == ReadSign::Minus) {
        // A single minus sign is technically a bare key.
        expectBareKeyEnd();
        return createToken(TokenType::BareKey);
    }
    _reader.throwUnexpectedCharacter();
}


void Tokenizer::readOptionalPlusMinusSign() {
    if (_reader.isPlusMinusSign()) {
        if (_reader.isPlus()) {
            _readSign = ReadSign::Plus;
        } else {
            _readSign = ReadSign::Minus;
        }
        _reader.consumeCharAndExpectMore();
    } else {
        _readSign = ReadSign::None;
    }
}


auto Tokenizer::readFloatBoolOrBareKey() -> Token {
    while (_reader.isBareKey()) {
        if (_reader.consumeChar() == StreamState::EndOfStream) {
            break;
        }
        if (_reader.tokenSize() > cBareKeyCharacterLimit) {
            _reader.throwSyntaxError(QStringLiteral("Bare key exceeds character limit."));
        }
    }
    if (_reader.tokenMatches(_floatSpecials)) {
        expectValueEnd();
        return createToken(TokenType::Float);
    }
    if (_reader.tokenMatches(_booleanValues)) {
        expectValueEnd();
        return createToken(TokenType::Boolean);
    }
    if (_readSign == ReadSign::Plus) {
        _reader.throwSyntaxError(QStringLiteral("Unknown identifier after plus sign."));
    }
    expectBareKeyEnd();
    return createToken(TokenType::BareKey);
}


auto Tokenizer::detectStringType() -> bool {
    // first detect the nature of the string to read.
    _stringQuotes = _reader.isLiteralString() ? StringQuotes::Literal : StringQuotes::Regular;
    _stringMode = StringMode::SingleLine;
    _reader.skipCharAndExpectMore();
    // 2. repeated string character?
    if (isString()) {
        if (_reader.skipCharAndTestAtEnd()) { // already at the end?
            return true; // it's an empty string.
        }
        // 3. repeated string character?
        if (isString()) {
            _reader.skipCharAndExpectMore();
            _stringMode = StringMode::MultiLine;
        } else {
            // there is something else ofter the second string quote, so this must be an empty string.
            return true;
        }
    }
    return false; // string contents will follow.
}


void Tokenizer::checkAndSkipNewlineAfterMultilineStart() {
    // Skip any newline directly following the initial multi line string quotes.
    if (isMultiLineString() && _reader.isNewLineOrCarriageReturn()) {
        _reader.expectMoreData(_reader.skipNewLine());
    }
}


auto Tokenizer::detectStringEnd() -> bool {
    if (_reader.skipCharAndTestAtEnd() && isMultiLineString()) { // if multiline, that's too early.
        _reader.throwPrematureEnd();
    }
    if (!isMultiLineString()) { // non multiline ends after
        return true;
    }
    int endStringCount = 1;
    while (isString()) {
        endStringCount += 1;
        if (endStringCount > 5) {
            _reader.throwSyntaxError(
                QStringLiteral("More than five end quotes are not allowed at the end of a multiline string."));
        }
        if (_reader.skipCharAndTestAtEnd()) {
            break;
        }
    }
    if (endStringCount >= 3) { // not sure what happens when >5
        for (int i = 0; i < (endStringCount - 3); ++i) {
            _reader.writeToToken(getStringChar());
        }
        return true;
    }
    for (int i = 0; i < endStringCount; ++i) {
        _reader.writeToToken(getStringChar());
    }
    return false;
}


void Tokenizer::readBackslashEscaped() {
    if (_stringQuotes == StringQuotes::Literal) { // no backslash handling for literal strings.
        _reader.consumeChar();
        return;
    }
    _reader.skipCharAndExpectMore(); // Skip the backslash
    // A backslash at the end of the line will skip any following whitespace and newlines.
    if (isMultiLineString() && (_reader.isNewLineOrCarriageReturn() || _reader.isWhiteSpace())) {
        if (_reader.isWhiteSpace()) {
            // special handling to whitespace, to make sure it's not an escaped space.
            while (_reader.isWhiteSpace()) {
                _reader.skipCharAndExpectMore();
            }
            // At this point there *must* be a newline
            if (!_reader.isNewLineOrCarriageReturn()) {
                _reader.throwSyntaxError(
                    QStringLiteral("Backslash with space or tab found that is not at the end of the line."));
            }
        }
        // Skip all white space up to the next non-whitespace character.
        while (_reader.isNewLineOrCarriageReturn() || _reader.isWhiteSpace()) {
            _reader.expectMoreData(_reader.skipNewLine());
        }
        return;
    }
    switch (_reader.currentChar().toAscii()) {
    case 'e':
        if (_specification >= Specification::Version_1_1) {
            _reader.skipCharWriteAndExpectMore(Char{'\x1b'}); // escape
        } else {
            _reader.throwUnexpectedCharacter();
        }
        break;
    case 'b':
        _reader.skipCharWriteAndExpectMore(Char{'\b'});
        break;
    case 't':
        _reader.skipCharWriteAndExpectMore(Char{'\t'});
        break;
    case 'n':
        _reader.skipCharWriteAndExpectMore(Char{'\n'});
        break;
    case 'f':
        _reader.skipCharWriteAndExpectMore(Char{'\f'});
        break;
    case 'r':
        _reader.skipCharWriteAndExpectMore(Char{'\r'});
        break;
    case '"':
        _reader.skipCharWriteAndExpectMore(Char{'"'});
        break;
    case '\\':
        _reader.skipCharWriteAndExpectMore(Char{'\\'});
        break;
    case 'x':
        if (_specification >= Specification::Version_1_1) {
            readUnicodeEscape(2);
        } else {
            _reader.throwUnexpectedCharacter();
        }
        break;
    case 'u':
        readUnicodeEscape(4);
        break;
    case 'U':
        readUnicodeEscape(8);
        break;
    default:
        _reader.throwUnexpectedCharacter();
    }
}


void Tokenizer::readUnicodeEscape(int count) {
    _reader.skipCharAndExpectMore(); // skip the 'U', 'u' or 'x'
    char32_t unicode{0};
    for (int i = 0; i < count; ++i) {
        unicode <<= 4;
        unicode |= _reader.skipHexDigit();
    }
    Char c{unicode};
    if (!c.isValidUnicode()) {
        _reader.throwSyntaxError(QStringLiteral("Invalid unicode value."));
    }
    _reader.writeToToken(c);
}


auto Tokenizer::readStringContent() -> Token {
    while (!_reader.atEnd()) {
        if (isString()) {
            if (detectStringEnd()) {
                return createToken(
                    _stringMode == StringMode::MultiLine ?
                    TokenType::MultiLineString :
                    TokenType::SingleLineString);
            }
        } else if (_reader.isBackslash()) {
            readBackslashEscaped();
        } else if (_reader.isNewLineOrCarriageReturn()) {
            if (isMultiLineString()) {
                _reader.expectMoreData(_reader.consumeNewLine());
            } else {
                _reader.throwSyntaxError(QStringLiteral("Newlines are not allowed in single line strings."));
            }
        } else if (_reader.isControlCharacter()) {
            _reader.throwSyntaxError(QStringLiteral("Control characters are not allowed in a string."));
        } else {
            _reader.consumeCharAndExpectMore();
        }
        if (_reader.tokenSize() > cStringCharacterLimit) {
            _reader.throwSyntaxError(QStringLiteral("The string exceeded the maximum allowed size."));
        }
    }
    // if we reach this point, it means the stream ended before the string was closed.
    _reader.throwPrematureEnd();
}


auto Tokenizer::readString() -> Token {
    if (detectStringType()) { // set the string type and check if it's empty.
        return createToken(
                _stringMode == StringMode::MultiLine ?
                TokenType::MultiLineString :
                TokenType::SingleLineString);
    }
    checkAndSkipNewlineAfterMultilineStart();
    return readStringContent();
}


auto Tokenizer::readBareKey() -> Token {
    while (_reader.isBareKey()) {
        if (_reader.consumeChar() == StreamState::EndOfStream) {
            break;
        }
        if (_reader.tokenSize() > cBareKeyCharacterLimit) {
            _reader.throwSyntaxError("Bare key exceeds character limit.");
        }
    }
    expectBareKeyEnd();
    return createToken(TokenType::BareKey);
}


auto Tokenizer::readNumberLike() -> Token {
    if (_reader.consumeChar() == StreamState::EndOfStream) {
        return createToken(TokenType::DecimalInteger); // 1 digit integer.
    }
    if (_reader.isNumberSystemPrefix()) {
        return readPrefixedNumber();
    }
    if (_reader.consumeDigits(NumberSystem::Decimal, true) == StreamState::EndOfStream) {
        return createToken(TokenType::DecimalInteger);
    }
    if (_reader.isDot()) {
        return readFloatFraction();
    }
    if (_reader.isExponentPrefix()) {
        return readExponent();
    }
    if ((_reader.isDateSeperator() || _reader.isTimeSeperator()) && _readSign == ReadSign::None) {
        return readDateOrTime();
    }
    expectValueEnd();
    return createToken(TokenType::DecimalInteger);
}


auto Tokenizer::readPrefixedNumber() -> Token {
    if (_reader.lastConsumed() != '0' || _readSign != ReadSign::None) {
        _reader.throwUnexpectedCharacter();
    }
    if (_reader.isHexPrefix()) {
        _reader.consumeCharAndExpectMore();
        _reader.consumeDigits(NumberSystem::Hexadecimal, false);
        expectValueEnd();
        return createToken(TokenType::HexInteger);
    }
    if (_reader.isOctalPrefix()) {
        _reader.consumeCharAndExpectMore();
        _reader.consumeDigits(NumberSystem::Octal, false);
        expectValueEnd();
        return createToken(TokenType::OctalInteger);
    }
    // binary prefix
    _reader.consumeCharAndExpectMore();
    _reader.consumeDigits(NumberSystem::Binary, false);
    expectValueEnd();
    return createToken(TokenType::BinaryInteger);
}


auto Tokenizer::readFloatFraction() -> Token {
    _reader.consumeCharAndExpectMore();
    if (!_reader.isDecimalDigit()) {
        _reader.throwUnexpectedCharacter();
    }
    if (_reader.consumeDigits(NumberSystem::Decimal, false) == StreamState::EndOfStream) {
        expectValueEnd();
        return createToken(TokenType::Float);
    }
    if (_reader.isExponentPrefix()) {
        return readExponent();
    }
    expectValueEnd();
    return createToken(TokenType::Float);
}


auto Tokenizer::readExponent() -> Token {
    _reader.consumeCharAndExpectMore();
    if (_reader.isPlusMinusSign()) {
        _reader.consumeCharAndExpectMore();
    }
    if (!_reader.isDecimalDigit()) {
        _reader.throwUnexpectedCharacter();
    }
    _reader.consumeDigits(NumberSystem::Decimal, false);
    expectValueEnd();
    return createToken(TokenType::Float);
}


auto Tokenizer::readDateOrTime() -> Token {
    if (_reader.isDateSeperator()) {
        return readDate();
    }
    return readTime();
}


auto Tokenizer::readDateSeperator() -> void {
    if (!_reader.isDateSeperator()) {
        _reader.throwUnexpectedCharacter();
    }
    _reader.consumeCharAndExpectMore();
}


auto Tokenizer::readTimeSeperator() -> void {
    if (!_reader.isTimeSeperator()) {
        _reader.throwUnexpectedCharacter();
    }
    _reader.consumeCharAndExpectMore();
}


auto Tokenizer::readOptionalFraction() -> StreamState {
    auto streamState = StreamState::MoreData;
    if (_reader.isDot()) {
        streamState = _reader.consumeChar();
        _reader.expectMoreData(streamState);
        for (int i = 0;; ++i) {
            if (_reader.isDecimalDigit()) {
                streamState = _reader.consumeChar();
                if (streamState == StreamState::EndOfStream) {
                    break;
                }
            } else {
                break;
            }
            if (i == 10) {
                _reader.throwSyntaxError(QStringLiteral("Too many digits for second fraction."));
            }
        }
    }
    return streamState;
}


auto Tokenizer::readTimeZone() -> TokenType {
    if (_reader.isUtcTimeZone()) {
        _reader.consumeChar();
        return TokenType::OffsetDateTime;
    }
    if (_reader.isPlusMinusSign()) {
        _reader.consumeCharAndExpectMore();
        _reader.consumeDecimalDigitsAndExpectMore(2);
        readTimeSeperator();
        if (_reader.consumeDecimalDigitsAndTestAtEnd(2)) {
            return TokenType::OffsetDateTime;
        }
        expectValueEnd();
        return TokenType::OffsetDateTime;
    }
    expectValueEnd();
    return TokenType::LocalDateTime;

}


auto Tokenizer::readDate() -> Token {
    if (_reader.tokenSize() != 4) {
        _reader.throwSyntaxError(QStringLiteral("Unexpected minus character after integer value."));
    }
    _reader.consumeCharAndExpectMore(); // consume the `-`
    _reader.consumeDecimalDigitsAndExpectMore(2);
    readDateSeperator();
    if (_reader.consumeDecimalDigitsAndTestAtEnd(2)) {
        return createToken(TokenType::LocalDate);
    }
    if (_reader.isDateAndTimeSeperator()) {
        if (_reader.isWhiteSpace()) {
            if (_reader.skipCharAndTestAtEnd()) {
                return createToken(TokenType::LocalDate);
            }
            if (!_reader.isDecimalDigit()) {
                expectValueEnd();
                return createToken(TokenType::LocalDate);
            }
            _reader.writeToToken(Char{' '}); // write the skipped space to the token buffer.
        } else {
            _reader.consumeCharAndExpectMore();
        }
        _reader.consumeDecimalDigitsAndExpectMore(2);
        readTimeSeperator();
        if (_reader.consumeDecimalDigitsAndTestAtEnd(2)) {
            return createToken(TokenType::LocalDate);
        }
        if (_reader.isTimeSeperator()) { // has seconds?
            _reader.consumeCharAndExpectMore();
            if (_reader.consumeDecimalDigitsAndTestAtEnd(2)) {
                return createToken(TokenType::LocalDate);
            }
            readOptionalFraction();
        } else if (_specification <= Specification::Version_1_0) {
            _reader.throwSyntaxError("Times without seconds are not supported in TOML 1.0.");
        }
        return createToken(readTimeZone());
    }
    expectValueEnd();
    return createToken(TokenType::LocalDate);
}


auto Tokenizer::readTime() -> Token {
    if (_reader.tokenSize() != 2) {
        _reader.throwSyntaxError(QStringLiteral("Unexpected colon after integer value."));
    }
    _reader.consumeCharAndExpectMore(); // consume the `:`
    _reader.consumeDecimalDigitsAndExpectMore(2);
    if (_reader.isTimeSeperator()) { // has seconds?
        _reader.consumeCharAndExpectMore();
        if (_reader.consumeDecimalDigitsAndTestAtEnd(2)) {
            return createToken(TokenType::LocalTime);
        }
        readOptionalFraction();
    } else if (_specification <= Specification::Version_1_0) {
        _reader.throwSyntaxError("Times without seconds are not supported in TOML 1.0.");
    }
    expectValueEnd();
    return createToken(TokenType::LocalTime);
}


void Tokenizer::expectValueEnd() {
    if (!_reader.isPossibleValueEnd()) { // improve the error messages
        _reader.throwSyntaxError(QStringLiteral("Unexpected character after this value."));
    }
}


void Tokenizer::expectBareKeyEnd() {
    if (_reader.atEnd()) {
        _reader.throwPrematureEnd(); // after a bare key, there has to be more content.
    }
    if (!_reader.isPossibleBareKeyEnd()) { // improve the error messages
        _reader.throwSyntaxError(QStringLiteral("Unexpected character after this bare key."));
    }
}


auto Tokenizer::isString() const noexcept -> bool {
    switch (_stringQuotes) {
    case StringQuotes::None:
    default:
        return _reader.isRegularString() || _reader.isLiteralString();
    case StringQuotes::Regular:
        return _reader.isRegularString();
    case StringQuotes::Literal:
        return _reader.isLiteralString();
    }
}


auto Tokenizer::getStringChar() const noexcept -> Char {
    switch (_stringQuotes) {
    case StringQuotes::None:
    default:
        return {};
    case StringQuotes::Regular:
        return Char{'\"'};
    case StringQuotes::Literal:
        return Char{'\''};
    }
}


auto Tokenizer::isMultiLineString() const noexcept -> bool {
    return _stringMode == StringMode::MultiLine;
}


}

