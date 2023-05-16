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


#include "../InputStream.hpp"


namespace erbsland::qt::toml::impl {


/// @private
/// A input stream that reads from a QString
///
class StringInputStream final : public InputStream {
public:
    /// Create a new string based input stream.
    ///
    /// @param text THe string with the text.
    ///
    explicit StringInputStream(QString text) noexcept;

    // defaults
    ~StringInputStream() override = default;

public: // implement InputStream
    [[nodiscard]] auto atEnd() noexcept -> bool override;
    [[nodiscard]] auto readOrThrow() -> Char override;
    [[nodiscard]] auto document() const noexcept -> QString override;

private:
    int _readPosition{}; ///< The read position in the string.
    QString _textCopy{}; ///< A copy of the text for the stream.
};


}
