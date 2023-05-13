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


#include "NumberSystem.hpp"
#include "StreamState.hpp"
#include "Token.hpp"
#include "TokenType.hpp"

#include "../InputStream.hpp"
#include "../LocationRange.hpp"
#include "../Specification.hpp"

#include <tuple>


namespace erbsland::qt::toml::impl {


/// @internal
/// A class specialized reading characters from the stream, classify them and assemble tokens.
///
/// This class provides all the low-level functionality of the tokenizer, without the
/// actual logic. While there is not a perfect clear separation between the reader and
/// the tokenizer, it helps to make the code of the tokenizer more readable.
///
class CharReader final {
private:
    /// The maximum number of digits of an integer/float (for the token).
    ///
    static constexpr int32_t cIntOrFloatCharacterLimit = 100;

public:
    /// Create a new character reader.
    ///
    /// @param specification The specification version to use.
    ///
    explicit CharReader(Specification specification) noexcept;

public: // low-level
    /// Access the input stream.
    ///
    inline auto inputStream() noexcept -> InputStreamPtr {
        return _stream;
    }

    /// Set the input stream.
    ///
    void resetWithInputStream(InputStreamPtr inputStream) noexcept;

    /// Access the last read character.
    ///
    [[nodiscard]] inline auto currentChar() const -> Char {
        return _char;
    }

    /// Read the next character from the stream.
    ///
    void readNextChar();

    /// Test if the stream is at the end.
    ///
    inline auto atEnd() noexcept -> bool {
        return _char.isNull() && _stream->atEnd();
    }

public: // Skip and consume characters.
    /// Skip the current character (do not add it to the token buffer) and read the next.
    ///
    /// @return The state of the stream after skipping the character.
    ///
    auto skipChar() -> StreamState;

    /// Skip the current character and test if we reached the end of the stream.
    ///
    /// @return `true` if we reached the end of the stream.
    ///
    inline auto skipCharAndTestAtEnd() -> bool {
        return skipChar() == StreamState::EndOfStream;
    }

    /// Skip the current character and expect more characters to come.
    ///
    inline void skipCharAndExpectMore() {
        expectMoreData(skipChar());
    }

    /// Skip the current character, write another instead and expect more data.
    ///
    inline void skipCharWriteAndExpectMore(Char newChar) {
        expectMoreData(skipCharAndWrite(newChar));
    }

    /// Consumes the current character (adding it to the token buffer) and reads the next.
    ///
    /// @return The state of the stream after consuming the character.
    ///
    auto consumeChar() -> StreamState;

    /// Consume the current character and expect more characters to come.
    ///
    inline void consumeCharAndExpectMore() {
        expectMoreData(consumeChar());
    }

    /// Get the last consumed character.
    ///
    /// @return The last consumed character.
    ///
    [[nodiscard]] auto lastConsumed() const noexcept -> QChar;

    /// Consume a LF or CRLF newline.
    ///
    /// @return The state of the stream after consuming the character.
    ///
    auto consumeNewLine() -> StreamState;

    /// Skip a LF or CRLF newline.
    ///
    /// @return The state of the stream after skipping the character.
    ///
    auto skipNewLine() -> StreamState;

    /// Skip whitespace (space or tab).
    ///
    /// @return The state of the stream after skipping the character.
    ///
    auto skipWhiteSpace() -> StreamState;

    /// Test if there is more data to read and throw exception if the end of the stream is reached.
    ///
    void expectMoreData(StreamState streamState);

    /// Skip the current character, read the next, and add another character to the buffer.
    ///
    /// @return The state of the stream after skipping the character.
    ///
    auto skipCharAndWrite(Char newChar) -> StreamState;

    /// Skip a hexadecimal digit, and return it's value or throw a syntax error.
    ///
    /// This function is used for the `\uXXXX` and `\UXXXXXXXX` sequences.
    ///
    /// @return The value of the digit.
    /// @throws Error if the character is no hex digit.
    ///
    [[nodiscard]] auto skipHexDigit() -> uint32_t;

    /// Consume digits of a given numeric system, allowing '_' seperator.
    ///
    auto consumeDigits(NumberSystem numberSystem, bool lastConsumedWasDigit) -> StreamState;

    /// Consume a number of decimal digits.
    ///
    [[nodiscard]] auto consumeDecimalDigits(int32_t count) -> StreamState;

    /// Consume a number of decimal digits and expect more after them.
    ///
    inline void consumeDecimalDigitsAndExpectMore(int32_t count) {
        expectMoreData(consumeDecimalDigits(count));
    }

    /// Consume a number of decimal digits and test if the end of the stream was reached.
    ///
    inline auto consumeDecimalDigitsAndTestAtEnd(int32_t count) -> bool {
        return consumeDecimalDigits(count) == StreamState::EndOfStream;
    }

public: // token buffer functions
    /// Get the current size of the token.
    ///
    [[nodiscard]] inline auto tokenSize() const noexcept -> qsizetype {
        return _token.size();
    }

    /// Access the current token buffer
    ///
    [[nodiscard]] inline auto token() const noexcept -> const QString& {
        return _token;
    }

    /// Test if a token matches one string in the given list.
    ///
    /// @return `true` if one of the strings in the list matches the token.
    ///
    [[nodiscard]] auto tokenMatches(const std::vector<const char*> &stringList) const noexcept -> bool;

    /// Get and clear the token buffer
    ///
    /// @return The buffer contents, the location range of the token.
    ///
    auto takeToken() noexcept -> std::tuple<QString, LocationRange>;

    /// Write a character to the token buffer.
    ///
    void writeToToken(Char newChar) noexcept;

public: // character testing
    [[nodiscard]] inline auto isWhiteSpace() const noexcept -> bool {
        return _char == 0x20U || _char == 0x09U;
    }
    [[nodiscard]] inline auto isComment() const noexcept -> bool {
        return _char == '#';
    }
    [[nodiscard]] inline auto isNewLine() const noexcept -> bool {
        return _char == 0x0aU;
    }
    [[nodiscard]] inline auto isCarriageReturn() const noexcept -> bool {
        return _char == 0x0dU;
    }
    [[nodiscard]] inline auto isNewLineOrCarriageReturn() const noexcept -> bool {
        return isNewLine() || isCarriageReturn();
    }
    [[nodiscard]] inline auto isAssignment() const noexcept -> bool {
        return _char == '=';
    }
    [[nodiscard]] inline auto isDot() const noexcept -> bool {
        return _char == '.';
    }
    [[nodiscard]] inline auto isComma() const noexcept -> bool {
        return _char == ',';
    }
    [[nodiscard]] inline auto isArrayBegin() const noexcept -> bool {
        return _char == '[';
    }
    [[nodiscard]] inline auto isArrayEnd() const noexcept -> bool {
        return _char == ']';
    }
    [[nodiscard]] inline auto isTableBegin() const noexcept -> bool {
        return _char == '{';
    }
    [[nodiscard]] inline auto isTableEnd() const noexcept -> bool {
        return _char == '}';
    }
    [[nodiscard]] inline auto isLiteralString() const noexcept -> bool {
        return _char == '\'';
    }
    [[nodiscard]] inline auto isRegularString() const noexcept -> bool {
        return _char == '\"';
    }
    [[nodiscard]] inline auto isBackslash() const noexcept -> bool {
        return _char == '\\';
    }
    [[nodiscard]] inline auto isControlCharacter() const noexcept -> bool {
        return (_char >= 0x00U && _char <= 0x08U)
            || (_char >= 0x0aU && _char <= 0x1fU)
            || (_char == 0x7fU);
    }
    [[nodiscard]] inline auto isDigit(NumberSystem numberSystem) const noexcept -> bool {
        switch (numberSystem) {
        case NumberSystem::Decimal:
        default:
            return isDecimalDigit();
        case NumberSystem::Binary:
            return isBinaryDigit();
        case NumberSystem::Octal:
            return isOctalDigit();
        case NumberSystem::Hexadecimal:
            return isHexDigit();
        }
    }
    [[nodiscard]] inline auto isDecimalDigit() const noexcept -> bool {
        return (_char >= '0' && _char <= '9');
    }
    [[nodiscard]] inline auto isHexDigit() const noexcept -> bool {
        return (_char >= 'A' && _char <= 'F')
               || (_char >= 'a' && _char <= 'f')
               || (_char >= '0' && _char <= '9');
    }
    [[nodiscard]] inline auto isOctalDigit() const noexcept -> bool {
        return (_char >= '0' && _char <= '7');
    }
    [[nodiscard]] inline auto isBinaryDigit() const noexcept -> bool {
        return (_char >= '0' && _char <= '1');
    }
    [[nodiscard]] auto isBareKey() const noexcept -> bool;
    [[nodiscard]] inline auto isUnderscore() const noexcept -> bool {
        return _char == '_';
    }
    [[nodiscard]] inline auto isNumberSystemPrefix() const noexcept -> bool {
        return _char == 'x' || _char == 'o' || _char == 'b';
    }
    [[nodiscard]] inline auto isHexPrefix() const noexcept -> bool {
        return _char == 'x';
    }
    [[nodiscard]] inline auto isOctalPrefix() const noexcept -> bool {
        return _char == 'o';
    }
    [[nodiscard]] inline auto isExponentPrefix() const noexcept -> bool {
        return _char == 'e' || _char == 'E';
    }
    [[nodiscard]] inline auto isPlus() const noexcept -> bool {
        return _char == '+';
    }
    [[nodiscard]] inline auto isPlusMinusSign() const noexcept -> bool {
        return _char == '+' || _char == '-';
    }
    [[nodiscard]] inline auto isDateSeperator() const noexcept -> bool {
        return _char == '-';
    }
    [[nodiscard]] inline auto isTimeSeperator() const noexcept -> bool {
        return _char == ':';
    }
    [[nodiscard]] inline auto isDateAndTimeSeperator() const noexcept -> bool {
        return _char == 'T' || _char == 't' || _char == ' ';
    }
    [[nodiscard]] inline auto isUtcTimeZone() const noexcept -> bool {
        return _char == 'z' || _char == 'Z';
    }
    [[nodiscard]] inline auto isPossibleValueEnd() const noexcept -> bool {
        return _stream->atEnd() || isWhiteSpace() || isNewLineOrCarriageReturn() || isComma() || isComment()
               || isTableEnd() || isArrayEnd();
    }
    [[nodiscard]] inline auto isPossibleBareKeyEnd() const noexcept -> bool {
        return isWhiteSpace() || isComment() || isDot() || isArrayEnd() || isAssignment();
    }

public: // throw exceptions
    /// Throw a syntax error.
    ///
    [[noreturn]] void throwSyntaxError(const QString& message);

    /// Throw a unexpected character syntax error.
    ///
    [[noreturn]] void throwUnexpectedCharacter();

    /// Throw a premature end syntax error.
    ///
    [[noreturn]] void throwPrematureEnd();

    /// Throw a number exceeds limits syntax error.
    ///
    [[noreturn]] void throwNumberExceedsLimits();

private:
    Specification _specification{}; ///< The version of the specification to use
    InputStreamPtr _stream{}; ///< The current assigned input stream.
    bool _hasChar{false}; ///< If a character was read from the stream.
    Char _char{}; ///< The last read character.
    Location _location{}; ///< The current read location.
    Location _startLocation{}; ///< The location at the token start.
    QString _token{}; ///< The current token.
};


}

