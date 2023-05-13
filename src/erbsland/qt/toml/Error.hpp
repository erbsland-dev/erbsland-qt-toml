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


#include "Namespace.hpp"
#include "Location.hpp"

#include <QtCore/QString>
#include <QtCore/QIODevice>

#include <cstdint>
#include <exception>


namespace erbsland::qt::toml {


/// An error thrown by the interfaces of the TOML library.
///
class Error final : public std::exception {
public:
    /// The type of the error.
    ///
    enum class Type {
        Generic,
        Syntax,
        Encoding,
        IO,
    };

public:
    /// Create a generic error with no message.
    ///
    Error() noexcept = default;

    /// Create a generic error.
    ///
    /// @param message A string containing the error message.
    ///
    explicit Error(QString message) noexcept;

private:
    /// Private ctor with all parameters.
    ///
    /// @param type The type of the error.
    /// @param document The document that caused the error (e.g. file path).
    /// @param location The location of the error.
    /// @param message The message of this error.
    ///
    Error(
        Type type,
        QString document,
        Location location,
        QString message) noexcept;

public:
    /// Creates an IO error with a specified document and IO device.
    ///
    /// @param document The document that caused the error (e.g. file path).
    /// @param ioDevice The QIODevice that caused the error. Error message is copied from this device.
    /// @return A new `Error` instance with the specified properties.
    ///
    static auto createIO(const QString &document, const QIODevice &ioDevice) noexcept -> Error;

    /// Creates an Encoding error with a specified document, line, and column.
    ///
    /// @param document The document that caused the error (e.g. file path).
    /// @param location The location of the error.
    /// @return A new `Error` instance with the specified properties.
    ///
    static auto createEncoding(const QString &document, Location location) noexcept -> Error;

    /// Creates a Syntax error with a specified document, line, column, and message.
    ///
    /// @param document The document that caused the error (e.g. file path).
    /// @param location The location of the error.
    /// @param message The message of this error.
    /// @return A new `Error` instance with the specified properties.
    ///
    static auto createSyntax(const QString &document, Location location, const QString &message) noexcept -> Error;

public:
    /// Get the document of the error.
    ///
    /// For strings, the document is `[string]`, for byte data it is `[data]` and for files
    /// is the path to the file as it was passed to the parser.
    ///
    /// @return The document for the error.
    ///
    [[nodiscard]] inline auto document() const noexcept -> QString {
        return _document;
    }

    /// Convert the error into a string.
    ///
    /// @return A string with all information of this error message.
    ///
    [[nodiscard]] auto toString() const noexcept -> QString;

public: // Implement std::exception
    [[nodiscard]] auto what() const noexcept -> const char* override;

private:
    /// Convert the given type to a string.
    ///
    /// @param type The type.
    /// @return A string for the type.
    ///
    static auto typeToString(Type type) noexcept -> QString;

private:
    Type _type{Type::Generic}; ///< The type of this error.
    QString _document{}; ///< The optional document that caused the error (e.g. file path).
    Location _location{Location::createNotSet()}; ///< The location of the error.
    QString _message{}; ///< The message of this error.
    mutable std::string _whatCache; ///< A cache for the implementation of the `what()` method.
};


}

