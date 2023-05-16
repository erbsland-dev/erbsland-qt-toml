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
#pragma once


#include "TextStreamInputStream.hpp"


namespace erbsland::qt::toml::impl {


/// @private
/// An input stream bases on byte data.
///
class DataInputStream final : public TextStreamInputStream {
public:
    /// Create a new input stream based on UTF-8 encoded byte data.
    ///
    /// @param data The array with the byte data.
    ///
    explicit DataInputStream(QByteArray data) noexcept;

public: // implement InputStream
    [[nodiscard]] auto document() const noexcept -> QString override;

private:
    QByteArray _dataCopy{}; ///< A copy of the data for the stream.
};


}

