// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "TextStreamInputStream.hpp"


#include "../Error.hpp"


namespace erbsland::qt::toml::impl {


TextStreamInputStream::TextStreamInputStream(InputStream::Type type) noexcept
    : InputStream(type) {
}


auto TextStreamInputStream::atEnd() noexcept -> bool {
    return _dataStream->atEnd();
}


auto TextStreamInputStream::readOrThrow() -> Char {
    if (atEnd()) {
        return {};
    }
    auto data = readByte();
    if (data < 0x80) {
        return Char{static_cast<uint32_t>(data)};
    }
    uint8_t cSize = 0;
    uint32_t unicodeValue;
    if ((data & 0b11100000U) == 0b11000000U) { // 2 Bytes
        if (data >= 0b11000010U) { // <127 values must not be encoded that way.
            cSize = 2;
            unicodeValue = (data & 0b00011111U);
        }
    } else if ((data & 0b11110000U) == 0b11100000U) { // 3 Bytes
        cSize = 3;
        unicodeValue = (data & 0b00001111U);
    } else if ((data & 0b11111000U) == 0b11110000U) { // 4 Bytes
        if (data < 0b11110101U) { // Value must not exceed 0x140000
            cSize = 4;
            unicodeValue = (data & 0b00000111U);
        }
    } // more than 4 following bytes is not valid for the UTF-8 encoding.
    if (cSize < 2) {
        throw Error::createEncoding(document(), {});
    }
    for (uint8_t i = 1; i < cSize; ++i) {
        if (atEnd()) {
            throw Error::createEncoding(document(), {});
        }
        data = readByte();
        if ((data & 0b11000000U) != 0b10000000U) {
            throw Error::createEncoding(document(), {});
        }
        unicodeValue <<= 6;
        unicodeValue |= (data & 0b00111111U);
    }
    Char readChar{unicodeValue};
    if (!readChar.isValidUnicode()) {
        throw Error::createEncoding(document(), {});
    }
    return readChar;
}


auto TextStreamInputStream::readByte() noexcept -> uint8_t {
    uint8_t result;
    (*_dataStream) >> result;
    return result;
}


}


