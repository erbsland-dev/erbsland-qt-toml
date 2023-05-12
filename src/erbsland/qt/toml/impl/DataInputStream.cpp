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

