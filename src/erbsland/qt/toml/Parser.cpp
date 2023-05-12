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


auto Parser::parseString(const QString &str) -> ValuePtr {
    return parseStream(InputStream::createFromString(str));
}


auto Parser::parseData(const QByteArray &data) -> ValuePtr {
    return parseStream(InputStream::createFromData(data));
}


auto Parser::parseFile(const QString &path) -> ValuePtr {
    return parseStream(InputStream::createFromFile(path));
}


auto Parser::parseStream(const InputStreamPtr &inputStream) -> ValuePtr {
    return d->parseStream(inputStream);
}


}

