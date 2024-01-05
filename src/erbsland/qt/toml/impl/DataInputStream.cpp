// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "DataInputStream.hpp"


#include <utility>


namespace erbsland::qt::toml::impl {


DataInputStream::DataInputStream(QByteArray data) noexcept
    : TextStreamInputStream{InputStream::Type::Data}, _dataCopy{std::move(data)} {

    _dataStream = std::make_unique<QDataStream>(&_dataCopy, QIODevice::ReadOnly);
}


auto DataInputStream::document() const noexcept -> QString {
    return QStringLiteral("[data]");
}


}

