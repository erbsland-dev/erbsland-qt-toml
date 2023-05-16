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

