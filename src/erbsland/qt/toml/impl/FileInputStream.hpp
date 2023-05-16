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
/// A input stream reading UTF-8 data from a file.
///
class FileInputStream final : public TextStreamInputStream {
public:
    /// Create a new file based stream.
    ///
    /// @param path The absolute path to the file.
    ///
    explicit FileInputStream(QString path);

public: // implement InputStream
    [[nodiscard]] auto document() const noexcept -> QString override;

private:
    QString _path{}; ///< The path to the file.
    std::unique_ptr<QFile> _file{}; ///< The file to read from.
};


}
