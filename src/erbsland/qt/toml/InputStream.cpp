// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "InputStream.hpp"


#include "impl/DataInputStream.hpp"
#include "impl/FileInputStream.hpp"
#include "impl/StringInputStream.hpp"

#include <memory>


namespace erbsland::qt::toml {


InputStream::InputStream(InputStream::Type type) noexcept : _type{type} {
}


auto InputStream::createFromString(const QString &text) noexcept -> InputStreamPtr {
    return std::make_unique<impl::StringInputStream>(text);
}


auto InputStream::createFromData(const QByteArray &data) noexcept -> InputStreamPtr {
    return std::make_unique<impl::DataInputStream>(data);
}


auto InputStream::createFromFileOrThrow(const QString &path) -> InputStreamPtr {
    return std::make_unique<impl::FileInputStream>(path);
}


}

