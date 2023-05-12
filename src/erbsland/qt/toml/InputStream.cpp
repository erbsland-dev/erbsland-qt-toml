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


auto InputStream::createFromFile(const QString &path) -> InputStreamPtr {
    return std::make_unique<impl::FileInputStream>(path);
}


}

