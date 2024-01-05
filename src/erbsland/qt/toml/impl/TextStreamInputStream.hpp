// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "../InputStream.hpp"

#include <QtCore/QDataStream>


namespace erbsland::qt::toml::impl {


/// @private
/// The common input stream type that uses a QTextStream
///
class TextStreamInputStream : public InputStream {
protected:
    /// Create a new QTextStream based input stream.
    ///
    /// @param type The type of the stream.
    ///
    explicit TextStreamInputStream(InputStream::Type type) noexcept;

public: // implement InputStream
    auto atEnd() noexcept -> bool override;
    auto readOrThrow() -> Char override;

private:
    auto readByte() noexcept -> uint8_t;

protected:
    std::unique_ptr<QDataStream> _dataStream{}; ///< The data stream to read the data.
};


}
