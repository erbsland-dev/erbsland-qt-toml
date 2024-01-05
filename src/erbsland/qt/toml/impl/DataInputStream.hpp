// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
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

