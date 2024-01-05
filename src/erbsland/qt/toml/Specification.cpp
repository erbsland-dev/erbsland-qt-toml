// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "Specification.hpp"


namespace erbsland::qt::toml {


auto specificationToString(Specification specification) noexcept -> QString {
    switch (specification) {
        case Specification::Version_1_0:
            return QStringLiteral("TOML 1.0");
        case Specification::Version_1_1:
            return QStringLiteral("TOML 1.1");
        default:
            return QStringLiteral("Unknown");
    }
}


}

