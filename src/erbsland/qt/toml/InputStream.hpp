// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include "Char.hpp"
#include "Namespace.hpp"

#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QChar>

#include <memory>


namespace erbsland::qt::toml {


class InputStream;
using InputStreamPtr = std::shared_ptr<InputStream>; ///< A shared pointer for the input stream.


/// A generic input stream to make this code more portable.
///
/// You can subclass this interface to implement a own stream source. If you subclass this interface,
/// please use the `Custom` type.
///
class InputStream {
    // fwd-entry: class InputStream

public:
    /// The type of this input stream.
    ///
    enum class Type {
        String, ///< Streaming from a string.
        Data, ///< Streaming from a block of byte data.
        File, ///< Streaming from a file.
        Custom, ///< A user implemented stream.
    };

public:
    /// dtor
    ///
    virtual ~InputStream() = default;

public:
    /// Get the type of this input stream.
    ///
    inline auto type() noexcept -> Type { return _type; }

public:
    /// Test if we reached the end of the stream.
    ///
    /// @return `true` if the end of the stream is reached.
    ///
    virtual auto atEnd() noexcept -> bool = 0;

    /// Get the next unicode character from the stream
    ///
    /// @return The unicode character.
    /// @throws Error if there is an encoding error in the data or an IO error while reading the file.
    ///
    virtual auto readOrThrow() -> Char = 0;

    /// Get a document string for an exception.
    ///
    [[nodiscard]] virtual auto document() const noexcept -> QString = 0;

public:
    /// Create a new input stream from the given text.
    ///
    /// @param text The text.
    /// @return The input stream.
    ///
    static auto createFromString(const QString &text) noexcept -> InputStreamPtr;

    /// Create a new input stream from given UTF-8 encoded binary data.
    ///
    /// @param data The data.
    /// @return The input stream.
    ///
    static auto createFromData(const QByteArray &data) noexcept -> InputStreamPtr;

    /// Create a new input stream for a file.
    ///
    /// @param path The path to the file.
    /// @return The input stream.
    /// @throws Error if the file does not exist or cannot be opened.
    ///
    static auto createFromFileOrThrow(const QString &path) -> InputStreamPtr;

protected:
    /// Create a new input stream of a given type.
    ///
    explicit InputStream(Type type) noexcept;

private:
    Type _type{Type::String}; ///< The type of the input stream.
};


}


