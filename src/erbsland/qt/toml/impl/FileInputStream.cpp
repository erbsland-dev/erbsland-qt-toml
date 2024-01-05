// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "FileInputStream.hpp"


#include "../Error.hpp"

#include <utility>


namespace erbsland::qt::toml::impl {


FileInputStream::FileInputStream(QString path)
    : TextStreamInputStream{InputStream::Type::File}, _path{std::move(path)} {

    _file = std::make_unique<QFile>(_path);
    if (!_file->open(QIODevice::ReadOnly)) {
        throw Error::createIO(_path, *_file);
    }
    _dataStream = std::make_unique<QDataStream>(_file.get());
}


auto FileInputStream::document() const noexcept -> QString {
    return _path;
}


}

