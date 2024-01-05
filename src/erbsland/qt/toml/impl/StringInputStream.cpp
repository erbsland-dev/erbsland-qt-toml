// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
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


