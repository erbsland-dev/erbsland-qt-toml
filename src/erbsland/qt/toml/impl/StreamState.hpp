// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


namespace erbsland::qt::toml::impl {


/// State after consuming or skipping a character sequence.
///
enum class StreamState {
    EndOfStream, ///< The end of the stream has been reached
    MoreData ///< There is more data in the stream.
};


}

