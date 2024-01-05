// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
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
