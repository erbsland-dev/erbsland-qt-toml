// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


namespace erbsland::qt {}


// Allow to disable the short namespace if it causes conflicts.
#ifndef ERBSLAND_NO_SHORT_NAMESPACE
#ifdef ERBSLAND_SHORT_NAMESPACE
namespace ERBSLAND_SHORT_NAMESPACE = erbsland;
#else
namespace el = erbsland;
#endif
#ifdef ERBSLAND_QT_SHORT_NAMESPACE
namespace ERBSLAND_QT_SHORT_NAMESPACE = erbsland::qt;
#else
namespace elqt = erbsland::qt;
#endif
#endif


/// @namespace erbsland
/// The base namespace for all Erbsland DEV libraries.

/// @namespace erbsland::qt
/// The base namespace for all Erbsland DEV libraries that are made for the Qt library.

/// @namespace erbsland::qt::toml
/// The namespace for the Erbsland Qt TOML library.

