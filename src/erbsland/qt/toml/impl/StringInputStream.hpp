// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
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
