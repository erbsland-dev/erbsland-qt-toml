// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


namespace erbsland::qt::toml {


/// The string format for the location.
///
enum class LocationFormat {
    Compact, ///< Generates a text in the format `[line]:[column]`.
    CompactWithIndex, ///< Generates a text in the format `[line]:[column]:([index])`.
    Long, ///< Generates a text in the format `line [line], column [column]`.
    LongWithIndex, ///< Generates a text in the format `line [line], column [column] (index [index])`.
};


}

