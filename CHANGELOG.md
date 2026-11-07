# Changelog

## 1.3.0 — 2026-11-07

- Require Qt 6 Core and propagate the C++17 requirement to consumers. Qt 5 support has ended permanently; earlier releases will receive no further updates.
- Use the current Qt date/time constructor on Qt 6.5 and newer while retaining compatibility with earlier Qt 6 versions.
- Continue maintenance for bug fixes, security fixes, and compatibility updates. No new features will be added because ErbslandDEV no longer uses Qt. Remove the roadmap.
- Update the development environment to Python 3.14 and refresh all Python dependencies, including packages affected by the reported security advisories. Record exact Python dependency versions in the requirements files.
- Update GitHub Actions to current releases pinned by commit SHA. Separate documentation builds from deployment, restrict token permissions, and add dependency audits and zizmor pedantic validation.
