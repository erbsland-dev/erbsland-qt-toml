// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "InputStream.hpp"
#include "Specification.hpp"
#include "Namespace.hpp"
#include "Value.hpp"
#include "Error.hpp"

#include <QtCore/QString>
#include <QtCore/QByteArray>


namespace erbsland::qt::toml {


namespace impl {
class ParserData;
}


/// The TOML parser.
///
class Parser final {
    // fwd-entry: class Parser

public:
    /// Create a new parser.
    ///
    /// @param specification The version of the specification to use for parsing.
    ///     Do not specify this value, unless you have fixed requirements for the parsed file. The default value
    ///     in this library will always be set to the latest official release of the TOML specification.
    ///
    explicit Parser(Specification specification = Specification::Version_1_0) noexcept;

    /// dtor
    ///
    ~Parser();

    // no copy and assignment.
    Parser(const Parser&) = delete;
    auto operator=(const Parser&) = delete;

public: // parse methods that throw exceptions.
    /// Parse TOML data from a string.
    ///
    /// @param str The string with the TOML data to parse.
    /// @return A value that contains the parsed TOML data. This is always the special *root table*.
    /// @throws Error in case of any problem when parsing the data.
    ///
    [[nodiscard]] auto parseStringOrThrow(const QString &str) -> ValuePtr;

    /// Parse TOML data from UTF-8 encoded data.
    ///
    /// @param data UTF-8 encoded data with TOML to parse.
    /// @return A value that contains the parsed TOML data. This is always the special *root table*.
    /// @throws Error in case of any problem when parsing the data.
    ///
    [[nodiscard]] auto parseDataOrThrow(const QByteArray &data) -> ValuePtr;

    /// Parse TOML data from a file.
    ///
    /// This function opens the file at `path` read-only, and reads the file as stream. It is the
    /// most efficient way to read TOML data from a file.
    ///
    /// @param path The absolute path to the file.
    /// @return A value that contains the parsed TOML data. This is always the special *root table*.
    /// @throws Error in case of any problem when parsing the data. If there is a problem with the
    ///    file, an `Error::Type::IO` error is thrown, that contains a description of the issue.
    ///
    [[nodiscard]] auto parseFileOrThrow(const QString &path) -> ValuePtr;

    /// Parse TOML data from an input stream.
    ///
    /// This function reads and parses data from the given input stream.
    ///
    /// @param inputStream The input stream.
    /// @return A value that contains the parsed TOML data. This is always the special *root table*.
    /// @throws Error from the stream implementation and on any problem with the data.
    ///
    [[nodiscard]] auto parseStreamOrThrow(const InputStreamPtr &inputStream) -> ValuePtr;

public: // parse methods that do not throw exceptions
    /// Parse TOML data from a string.
    ///
    /// @param str The string with the TOML data to parse.
    /// @return A value that contains the parsed TOML data, or a nullptr if there was an error.
    ///     You can access the last error using the `lastError` method.
    ///
    [[nodiscard]] auto parseString(const QString &str) noexcept -> ValuePtr;

    /// Parse TOML data from UTF-8 encoded data.
    ///
    /// @param data UTF-8 encoded data with TOML to parse.
    /// @return A value that contains the parsed TOML data, or a nullptr if there was an error.
    ///     You can access the last error using the `lastError` method.
    ///
    [[nodiscard]] auto parseData(const QByteArray &data) noexcept -> ValuePtr;

    /// Parse TOML data from a file.
    ///
    /// This function opens the file at `path` read-only, and reads the file as stream. It is the
    /// most efficient way to read TOML data from a file.
    ///
    /// @param path The absolute path to the file.
    /// @return A value that contains the parsed TOML data, or a nullptr if there was an error.
    ///     You can access the last error using the `lastError` method.
    ///
    [[nodiscard]] auto parseFile(const QString &path) noexcept -> ValuePtr;

    /// Parse TOML data from an input stream.
    ///
    /// This function reads and parses data from the given input stream.
    ///
    /// @param inputStream The input stream.
    /// @return A value that contains the parsed TOML data, or a nullptr if there was an error.
    ///     You can access the last error using the `lastError` method.
    ///
    [[nodiscard]] auto parseStream(const InputStreamPtr &inputStream) noexcept -> ValuePtr;

    /// Access the last error from a parse method call.
    ///
    /// @return The last error from one of the parse method. When called after a successful call,
    ///    or before parsing was done, the behaviour is save but undefined.
    ///
    [[nodiscard]] auto lastError() const noexcept -> const Error&;

private:
    impl::ParserData *d; ///< The implementation of the parser.
};


}

