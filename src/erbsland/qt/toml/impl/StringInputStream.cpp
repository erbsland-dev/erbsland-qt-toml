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
#include "StringInputStream.hpp"


#include "../Error.hpp"

#include <utility>


namespace erbsland::qt::toml::impl {


StringInputStream::StringInputStream(QString text) noexcept
    : InputStream(InputStream::Type::String), _textCopy{std::move(text)} {
}


auto StringInputStream::document() const noexcept -> QString {
    return QStringLiteral("[string]");
}


auto StringInputStream::atEnd() noexcept -> bool {
    return _readPosition >= _textCopy.size();
}


auto StringInputStream::readOrThrow() -> Char {
    if (atEnd()) {
        return {};
    }
    auto character1 = _textCopy.at(_readPosition++);
    Char readChar;
    if (!character1.isHighSurrogate()) {
        readChar = Char{static_cast<uint32_t>(character1.unicode())};
        if (!readChar.isValidUnicode()) {
            throw Error::createEncoding(document(), {});
        }
        return readChar;
    }
    if (character1.isLowSurrogate()) {
        throw Error::createEncoding(document(), {});
    }
    if (atEnd()) {
        throw Error::createEncoding(document(), {});
    }
    auto character2 = _textCopy.at(_readPosition++);
    if (character2.isHighSurrogate()) {
        throw Error::createEncoding(document(), {});
    }
    readChar = Char(static_cast<uint32_t>(QChar::surrogateToUcs4(character1, character2)));
    if (!readChar.isValidUnicode()) {
        throw Error::createEncoding(document(), {});
    }
    return readChar;
}


}


