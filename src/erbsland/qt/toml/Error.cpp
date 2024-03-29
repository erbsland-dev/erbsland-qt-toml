// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "Error.hpp"


#include <utility>


namespace erbsland::qt::toml {


Error::Error(QString message) noexcept
    : _message{std::move(message)} {
}


Error::Error(Error::Type type, QString document, Location location, QString message) noexcept
    : _type{type}, _document{std::move(document)}, _location{location}, _message{std::move(message)} {

}


auto Error::createIO(const QString &document, const QIODevice &ioDevice) noexcept -> Error {
    return {Error::Type::IO, document, {-1, -1, -1}, ioDevice.errorString()};
}


auto Error::createEncoding(const QString &document, Location location) noexcept -> Error {
    return {Error::Type::Encoding, document, location, QStringLiteral("Corrupted data in UTF-8 encoding.")};
}


auto Error::createSyntax(const QString &document, Location location, const QString &message) noexcept -> Error {
    return {Error::Type::Syntax, document, location, message};
}


auto Error::toString() const noexcept -> QString {
    QString result;

    // Append type information
    result.append(typeToString(_type));

    // Append document information if available
    if (!_document.isEmpty()) {
        result.append(QStringLiteral(" in "));
        result.append(_document);
    }

    // Append line and column information if available
    if (!_location.isNotSet()) {
        result.append(QStringLiteral(" at line "));
        result.append(QString::number(_location.line()));
        result.append(QStringLiteral(", column "));
        result.append(QString::number(_location.column()));
        result.append(QStringLiteral(" (index "));
        result.append(QString::number(_location.index()));
        result.append(QStringLiteral(")"));
    }

    // Append error message if available
    if (!_message.isEmpty()) {
        result.append(QStringLiteral(": "));
        result.append(_message);
    }

    return result;
}


auto Error::typeToString(Error::Type type) noexcept -> QString {
    switch (type) {
    case Type::Generic:
        return QStringLiteral("Generic");
    case Type::Syntax:
        return QStringLiteral("Syntax");
    case Type::Encoding:
        return QStringLiteral("Encoding");
    case Type::IO:
        return QStringLiteral("IO");
    default:
        return QStringLiteral("Unknown");
    }
}


auto Error::what() const noexcept -> const char * {
    if (_whatCache.empty()) {
        _whatCache = toString().toStdString();
    }
    return _whatCache.c_str();
}


}