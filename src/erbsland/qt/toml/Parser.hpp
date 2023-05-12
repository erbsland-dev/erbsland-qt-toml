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


#include "InputStream.hpp"
#include "Specification.hpp"
#include "Namespace.hpp"
#include "Value.hpp"

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

public:
    /// Parse TOML data from a string.
    ///
    /// @param str The string with the TOML data to parse.
    /// @return A value that contains the parsed TOML data. This is always the special *root table*.
    /// @throws Error in case of any problem when parsing the data.
    ///
    [[nodiscard]] auto parseString(const QString &str) -> ValuePtr;

    /// Parse TOML data from UTF-8 encoded data.
    ///
    /// @param data UTF-8 encoded data with TOML to parse.
    /// @return A value that contains the parsed TOML data. This is always the special *root table*.
    /// @throws Error in case of any problem when parsing the data.
    ///
    [[nodiscard]] auto parseData(const QByteArray &data) -> ValuePtr;

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
    [[nodiscard]] auto parseFile(const QString &path) -> ValuePtr;

    /// Parse TOML data from an input stream.
    ///
    /// This function reads and parses data from the given input stream.
    ///
    /// @param inputStream The input stream.
    /// @return A value that contains the parsed TOML data. This is always the special *root table*.
    /// @throws Error from the stream implementation and on any problem with the data.
    ///
    [[nodiscard]] auto parseStream(const InputStreamPtr &inputStream) -> ValuePtr;

private:
    impl::ParserData *d; ///< The implementation of the parser.
};


}

