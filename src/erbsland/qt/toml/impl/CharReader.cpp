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
#include "CharReader.hpp"


#include "../Error.hpp"


namespace erbsland::qt::toml::impl {


CharReader::CharReader(Specification specification) noexcept : _specification{specification} {
}


void CharReader::resetWithInputStream(InputStreamPtr inputStream) noexcept {
    _stream = std::move(inputStream);
    _hasChar = false;
    _char = {};
    _location = {};
    _token.clear();
    _token.reserve(128);
    _startLocation = {};
}


auto CharReader::takeToken() noexcept -> std::tuple<QString, LocationRange> {
    auto result = std::make_tuple(_token, LocationRange{_startLocation, _location});
    _token.clear();
    _startLocation = _location;
    _token.clear();
    return result;
}


void CharReader::readNextChar() {
    if (!_hasChar) {
        try {
            _char = _stream->read();
        } catch (const Error &error) {
            throw Error::createEncoding(error.document(), _location);
        }
        _hasChar = !(_char.isNull() && _stream->atEnd());
    }
}


auto CharReader::consumeChar() -> StreamState {
    _char.appendToString(_token);
    return skipChar();
}


auto CharReader::skipChar() -> StreamState {
    _location.increment(_char == 0x0aU);
    try {
        _char = _stream->read();
    } catch (const Error &error) {
        throw Error::createEncoding(error.document(), _location);
    }
    _hasChar = !(_char.isNull() && _stream->atEnd());
    return _hasChar ? StreamState::MoreData : StreamState::EndOfStream;
}


auto CharReader::lastConsumed() const noexcept -> QChar {
    if (_token.isEmpty()) {
        return {};
    }
    return _token.at(_token.size() - 1);
}


auto CharReader::consumeNewLine() -> StreamState {
    if (isCarriageReturn()) {
        expectMoreData(skipChar()); // never store the carriage return, to get a normalized text with newlines.
        if (!isNewLine()) {
            throwSyntaxError(QStringLiteral("Unexpected character after carriage return."));
        }
    }
    return consumeChar();
}


auto CharReader::skipNewLine() -> StreamState {
    if (isCarriageReturn()) {
        expectMoreData(skipChar());
        if (!isNewLine()) {
            throwSyntaxError(QStringLiteral("Unexpected character after carriage return."));
        }
    }
    return skipChar();
}


auto CharReader::skipWhiteSpace() -> StreamState {
    while (isWhiteSpace()) {
        if (skipChar() == StreamState::EndOfStream) {
            return StreamState::EndOfStream;
        }
    }
    if (_stream->atEnd()) {
        return StreamState::EndOfStream;
    }
    return StreamState::MoreData;
}


void CharReader::expectMoreData(StreamState streamState) {
    if (streamState == StreamState::EndOfStream) {
        throwPrematureEnd();
    }
}


void CharReader::writeToToken(Char newChar) noexcept {
    newChar.appendToString(_token);
}


auto CharReader::skipCharAndWrite(Char newChar) -> StreamState {
    auto result = skipChar();
    writeToToken(newChar);
    return result;
}


auto CharReader::skipHexDigit() -> uint32_t {
    uint32_t result{0};
    if (_char >= '0' && _char <= '9') {
        result = static_cast<uint32_t>(_char.toAscii() - '0');
    } else if (_char >= 'a' && _char <= 'f') {
        result = static_cast<uint32_t>(_char.toAscii() - 'a' + 0xaU);
    } else if (_char >= 'A' && _char <= 'F') {
        result = static_cast<uint32_t>(_char.toAscii() - 'A' + 0xaU);
    } else {
        throwUnexpectedCharacter();
    }
    expectMoreData(skipChar());
    return result;
}


auto CharReader::consumeDigits(NumberSystem numberSystem, bool lastConsumedWasDigit) -> StreamState {
    while(isDigit(numberSystem) || isUnderscore()) {
        if (isUnderscore()) {
            if (!lastConsumedWasDigit) {
                throwUnexpectedCharacter(); // Two subsequent '_' are not allowed. Must not start with '_'.
            }
            expectMoreData(skipChar()); // Skip the '_' characters, more has to come.
            lastConsumedWasDigit = false;
        } else {
            if (consumeChar() == StreamState::EndOfStream) {
                return StreamState::EndOfStream;
            }
            lastConsumedWasDigit = true;
        }
        if (_token.size() > cIntOrFloatCharacterLimit) {
            throwNumberExceedsLimits();
        }
    }
    if (!lastConsumedWasDigit) {
        throwSyntaxError(QStringLiteral("The last character in a number must not be an underscore."));
    }
    if (_stream->atEnd()) {
        return StreamState::EndOfStream;
    }
    return StreamState::MoreData;
}


auto CharReader::consumeDecimalDigits(int32_t count) -> StreamState {
    auto state = StreamState::MoreData;
    for (int32_t i = 0; i < count; ++i) {
        if (state == StreamState::EndOfStream) {
            throwPrematureEnd();
        }
        if (!isDecimalDigit()) {
            throwUnexpectedCharacter();
        }
        state = consumeChar();
    }
    return state;
}


void CharReader::throwSyntaxError(const QString& message) {
    throw Error::createSyntax(_stream->document(), _location, message);
}


void CharReader::throwUnexpectedCharacter() {
    throwSyntaxError(QStringLiteral("Read unexpected character"));
}


void CharReader::throwPrematureEnd() {
    throwSyntaxError(QStringLiteral("Unexpected end of data"));
}


void CharReader::throwNumberExceedsLimits() {
    throwSyntaxError(QStringLiteral("Number exceeds maximum digit limit."));
}


auto CharReader::tokenMatches(const std::vector<const char *> &stringList) const noexcept -> bool {
    return std::any_of(stringList.cbegin(), stringList.cend(), [&](const char *str) -> bool {
        return _token == str;
    });
}


auto CharReader::isBareKey() const noexcept -> bool {
    if (_specification >= Specification::Version_1_1) {
        // From the ABNF file
        return (_char >= 'a' && _char <= 'z')
               || (_char >= 'A' && _char <= 'Z')
               || (_char >= '0' && _char <= '9')
               || (_char == 0x2dU) // -
               || (_char == 0x5fU) // _
               || (_char >= 0xB2U && _char <= 0xB3U) // superscript digits
               || (_char == 0xB9U)
               || (_char >= 0xBCU && _char <= 0xBEU) // fractions
               || (_char >= 0xC0U && _char <= 0xD6U) // non-symbol chars in Latin block
               || (_char >= 0xD8U && _char <= 0xF6U)
               || (_char >= 0xF8U && _char <= 0x37DU)
               || (_char >= 0x37FU && _char <= 0x1FFFU) // exclude GREEK QUESTION MARK
               || (_char >= 0x200CU && _char <= 0x200DU) // ZWNJ, ZWJ
               || (_char >= 0x203FU && _char <= 0x2040U) // tie symbols
               || (_char >= 0x2070U && _char <= 0x218FU) // super-/subscripts
               || (_char >= 0x2460U && _char <= 0x24FFU) // letterlike/numberlike forms, enclosed alphanumerics
               || (_char >= 0x2C00U && _char <= 0x2FEFU) // skip arrows, math, box drawing etc
               || (_char >= 0x3001U && _char <= 0xD7FFU) // skip 2FF0-3000 ideographic up/down markers and spaces
               || (_char >= 0xF900U && _char <= 0xFDCFU) // skip D800-DFFF surrogate block
               || (_char >= 0xFDF0U && _char <= 0xFFFDU) // skip E000-F8FF Private Use area, FDD0-FDEF intended for process-internal use (unicode)
               || (_char >= 0x10000U && _char <= 0xEFFFFU); // all chars outside BMP range, excluding Private Use planes
    }
    return (_char >= 'a' && _char <= 'z')
           || (_char >= 'A' && _char <= 'Z')
           || (_char >= '0' && _char <= '9')
           || _char == '_'
           || _char == '-';
}


}

