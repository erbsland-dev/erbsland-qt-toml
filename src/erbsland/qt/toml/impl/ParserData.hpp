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


#include "Tokenizer.hpp"
#include "Token.hpp"

#include "../Specification.hpp"
#include "../Value.hpp"

#include <optional>


namespace erbsland::qt::toml::impl {


/// @private
/// The internal implementation of the parser.
///
class ParserData {
public:
    /// Create a new parser implementation.
    ///
    /// @param specification The specification version to use.
    ///
    explicit ParserData(Specification specification) noexcept;

    // defaults
    ~ParserData() = default;

public:
    /// Parse TOML data from an input stream.
    ///
    /// This function reads and parses data from the given input stream.
    ///
    /// @param inputStream The input stream.
    /// @return A value that contains the parsed TOML data. This is always the special *root table*.
    /// @throws Error from the stream implementation and on any problem with the data.
    ///
    [[nodiscard]] auto parseStream(const InputStreamPtr &inputStream) -> ValuePtr;

    /// Parse the tokens from the tokenizer.
    ///
    void parseDocument();

    /// Parse an assignment on the document level.
    ///
    void parseDocumentLevelAssignment();

    /// Parse a key = value assignment on the document level or in a local table.
    ///
    void parseKeyValueAssignment();

    /// Parse a table name.
    ///
    void parseTableName();

    /// Parse an array name.
    ///
    void parseArrayOfTablesName();

    /// Read the next non whitespace non comment token.
    ///
    void readNextToken();

    /// Read and require a next non whitespace non comment token.
    ///
    void readAndRequireNextToken();

    /// Parse a value.
    ///
    [[nodiscard]] auto parseValue() -> ValuePtr;

    /// Parse an integer value.
    ///
    [[nodiscard]] auto parseIntegerValue() -> ValuePtr;

    /// Parse a floating point value.
    ///
    [[nodiscard]] auto parseFloatValue() -> ValuePtr;

    /// Parse a time value.
    ///
    [[nodiscard]] auto parseTimeValue() -> ValuePtr;

    /// Parse a date time value.
    ///
    [[nodiscard]] auto parseDateTimeValue() -> ValuePtr;

    /// Verify and convert a date text.
    ///
    [[nodiscard]] auto convertDate(const QStringView &text) -> QDate;

    /// Verify and convert a time text.
    ///
    [[nodiscard]] auto convertTime(const QStringView &text) -> std::tuple<QTime, Qt::TimeSpec, int>;

    /// Parse an array.
    ///
    [[nodiscard]] auto parseArrayValue() -> ValuePtr;

    /// Parse an inline table.
    ///
    [[nodiscard]] auto parseInlineTableValue() -> ValuePtr;

    /// Create a new table.
    ///
    /// If the table already exists a syntax error exception is thrown.
    /// If an intermediate path to the table points to a non table element, there is always an exception thrown.
    /// The new table is set as current table.
    ///
    /// @param keys The vector with names tokens.
    /// @throws Error if the new table cannot be created.
    ///
    void createTable(std::vector<Token> keys);

    /// Create or extend an array.
    ///
    /// If the array already exists, it is extended by one table.
    /// The new table in the array is set as current table.
    ///
    /// @param keys The vector with names tokens.
    /// @throws Error if the new array cannot be created/extended.
    ///
    void createArrayOfTables(std::vector<Token> keys);

    /// Create intermediate name elements.
    ///
    /// Create all intermediate name elements in `names` starting from the `baseTable`. As described in the
    /// specification, if a name exists, but isn't a table or array, a syntax error is thrown.
    /// If a name is an existing table, it is used as these are intermediate elements.
    /// If a name is an existing array, the last element is used, if the last element is a table.
    /// If the name does not exist, a new empty table is created.
    ///
    /// @param keys The list with intermediate names tokens. If empty `baseTable` is returned.
    /// @param baseTable The base table from where to start creating the intermediate elements.
    /// @return The table of the last intermediate element.
    ///
    auto createIntermediateNameElements(
        const std::vector<Token>& keys,
        const ValuePtr &baseTable,
        bool isValueAssignment) -> ValuePtr;

    /// Try to assign a value to the current container.
    ///
    /// @param keys The vector with names tokens.
    /// @param value The value to assign.
    ///
    void assignValue(std::vector<Token> keys, const ValuePtr &value);

    /// Throw a syntax error.
    ///
    /// @param message The error message.
    /// @param token The token that caused the error. Empty = last read token.
    ///
    [[noreturn]] void throwSyntaxError(const QString &message, std::optional<Token> token = {});

    /// Access the last error from a parse method call.
    ///
    /// @return The last error from one of the parse method. When called after a successful call,
    ///    or before parsing was done, the behaviour is save but undefined.
    ///
    [[nodiscard]] inline auto lastError() const noexcept -> const Error& {
        return _lastError;
    }

private:
    Specification _specification{}; ///< The version of the specification to use
    Tokenizer _tokenizer; ///< The tokenizer used by this parser.
    Token _token{}; ///< The current token.
    ValuePtr _document{}; ///< The current document.
    ValuePtr _currentTable{}; ///< The current table.
    Error _lastError{}; ///< The last error from one of the parse method calls.
};


}

