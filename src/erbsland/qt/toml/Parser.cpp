// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "Parser.hpp"


#include "impl/ParserData.hpp"


namespace erbsland::qt::toml {


Parser::Parser(Specification specification) noexcept : d{new impl::ParserData{specification}} {
}


Parser::~Parser() {
    delete d;
}


auto Parser::parseStringOrThrow(const QString &str) -> ValuePtr {
    return parseStreamOrThrow(InputStream::createFromString(str));
}


auto Parser::parseDataOrThrow(const QByteArray &data) -> ValuePtr {
    return parseStreamOrThrow(InputStream::createFromData(data));
}


auto Parser::parseFileOrThrow(const QString &path) -> ValuePtr {
    return parseStreamOrThrow(InputStream::createFromFileOrThrow(path));
}


auto Parser::parseStreamOrThrow(const InputStreamPtr &inputStream) -> ValuePtr {
    return d->parseStream(inputStream);
}


auto Parser::parseString(const QString &str) noexcept -> ValuePtr {
    try {
        return parseStringOrThrow(str);
    } catch (const Error &error) {
        return {};
    }
}


auto Parser::parseData(const QByteArray &data) noexcept -> ValuePtr {
    try {
        return parseDataOrThrow(data);
    } catch (const Error &error) {
        return {};
    }
}


auto Parser::parseFile(const QString &path) noexcept -> ValuePtr {
    try {
        return parseFileOrThrow(path);
    } catch (const Error &error) {
        return {};
    }
}


auto Parser::parseStream(const InputStreamPtr &inputStream) noexcept -> ValuePtr {
    try {
        return parseStreamOrThrow(inputStream);
    } catch (const Error &error) {
        return {};
    }
}


auto Parser::lastError() const noexcept -> const Error& {
    return d->lastError();
}


}

