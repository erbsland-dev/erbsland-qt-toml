// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "CharReader.hpp"

#include "../Parser.hpp"

#include <tuple>


namespace erbsland::qt::toml::impl {


/// @private
/// A class specialized to detect and read tokens from the stream.
///
class Tokenizer final {
private:
    /// The maximum number of nested structures.
    ///
    static constexpr int32_t cValueNestingLimit = 20;

    /// The maximum length of a bare key.
    ///
    static constexpr int32_t cBareKeyCharacterLimit = 100;

    /// The maximum length of a string.
    ///
    static constexpr int32_t cStringCharacterLimit = 100'000'000;

private:
    /// The context for reading a token.
    ///
    enum class TokenContext {
        Structure, ///< Expecting structure data.
        Value ///< Expecting value data.
    };

    /// The string quotes.
    ///
    enum class StringQuotes {
        None,
        Regular,
        Literal,
    };

    /// The string mode.
    ///
    enum class StringMode {
        None, ///< Not inside of a string.
        SingleLine, ///< Inside of a single line string.
        MultiLine, ///< Inside of a multi line string.
    };

    /// A sign that was read for an integer or float value.
    ///
    enum class ReadSign {
        None, ///< No sign was read for the current integer/float value.
        Plus, ///< A plus sign was read.
        Minus, ///< A minus sign was read.
    };

public:
    explicit Tokenizer(Specification specification = Specification::Version_1_0) noexcept;

public: // high-level interface.
    /// Resets the tokenizer and assigns a new stream to tokenize.
    ///
    void startWithStream(const InputStreamPtr &inputStream) noexcept;

    /// Stops the tokenizer and frees the stream.
    ///
    void stop();

    /// Read the next token from the stream.
    ///
    /// @return The next token.
    /// @throws Error for IO and syntax errors.
    ///
    auto read() -> Token;

    /// Access the current input stream.
    ///
    inline auto inputStream() noexcept -> InputStreamPtr { return _reader.inputStream(); }

private: // token level functions.
    /// Create a token and clear the token buffer.
    ///
    auto createToken(TokenType tokenType) -> Token;

    /// Read a next token in the structure context.
    ///
    auto readStructure() -> Token;

    /// Read the next token in the value context.
    ///
    auto readValue() -> Token;

    /// Read a block of whitespace.
    ///
    auto readWhiteSpace() -> Token;

    /// Read a comment.
    ///
    auto readComment() -> Token;

    /// Read a string.
    ///
    auto readString() -> Token;

    /// Read an optional plus or minus sign.
    ///
    void readOptionalPlusMinusSign();

    /// Read a float special, bool or bare key.
    ///
    auto readFloatBoolOrBareKey() -> Token;

    /// Read an number like value.
    ///
    /// Can be an integer, float, hex, octal, binary number or a date or time.
    ///
    auto readNumberLike() -> Token;

    /// Read the exponent of a float value.
    ///
    auto readExponent() -> Token;

    /// Read the fraction of a float value.
    ///
    auto readFloatFraction() -> Token;

    /// Read a prefixed number (`0x...`).
    ///
    auto readPrefixedNumber() -> Token;

    /// Read a date or time.
    ///
    auto readDateOrTime() -> Token;

    /// Read a date.
    ///
    auto readDate() -> Token;

    /// Read a time.
    ///
    auto readTime() -> Token;

    /// Expect and read a date seperator.
    ///
    void readDateSeperator();

    /// Expect and read a time seperator
    ///
    void readTimeSeperator();

    /// Optionally read a second fraction.
    ///
    auto readOptionalFraction() -> StreamState;

    /// Read a time zone `z` or offset.
    ///
    auto readTimeZone() -> TokenType;

    /// Detect string type.
    ///
    /// @return `true` if this is an empty string.
    ///
    [[nodiscard]] auto detectStringType() -> bool;

    /// Detect and skip a newline that directly follows the open quotes of a multiline string.
    ///
    void checkAndSkipNewlineAfterMultilineStart();

    /// Read a backslash escaped sequence inside of a string.
    ///
    void readBackslashEscaped();

    /// Read a unicode backslash sequence.
    ///
    /// @param count The number of digits to expect.
    ///
    void readUnicodeEscape(int count);

    /// Read the contents of a string after the open quotes, including the ending quotes.
    ///
    [[nodiscard]] auto readStringContent() -> Token;

    /// Detect the end of a string.
    ///
    /// @return `true` if the end of the string was detected.
    ///
    [[nodiscard]] auto detectStringEnd() -> bool;

    /// Read a bare key.
    ///
    auto readBareKey() -> Token;

    /// Expect that we are at the end of a value. If not, throw an exception.
    ///
    void expectValueEnd();

    /// Expect that we are at the end of a bare key. If not, throw an exception.
    ///
    void expectBareKeyEnd();

public: // string handling.
    /// Test if the current character is a string for the current string context.
    ///
    /// @return `true` if its a string character.
    ///
    [[nodiscard]] auto isString() const noexcept -> bool;

    /// Get the string character for the current context.
    ///
    [[nodiscard]] auto getStringChar() const noexcept -> Char;

    /// Test if we are currently in a multiline string.
    ///
    [[nodiscard]] auto isMultiLineString() const noexcept -> bool;

private:
    Specification _specification{}; ///< The version of the specification to use
    CharReader _reader; ///< The character reader.
    // constants
    static const std::vector<const char*> _floatSpecials; ///< Float special values
    static const std::vector<const char*> _booleanValues; ///< Boolean values.
    // context
    TokenContext _tokenContext{TokenContext::Structure}; ///< The current token context;
    int32_t _valueNestingCount{0}; ///< The number of nested levels of tables and arrays in the value.
    // token variables.
    StringQuotes _stringQuotes{StringQuotes::None}; ///< The current string quotes.
    StringMode _stringMode{StringMode::None}; ///< The current string mode.
    ReadSign _readSign{ReadSign::None}; ///< A sign that was read in front of an integer or float.
};


}