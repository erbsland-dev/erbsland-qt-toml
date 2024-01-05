// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once


#include <memory>


namespace erbsland::qt::toml {


class Value;
using ValuePtr = std::shared_ptr<Value>;


/// An iterator to iterate over an array value.
///
class ValueIterator {
    friend class Value;

public: // definitions to satisfy the iterator concept.
    /// @private
    using iterator_category = std::forward_iterator_tag;
    /// @private
    using value_type = ValuePtr;
    /// @private
    using difference_type = std::ptrdiff_t;
    /// @private
    using pointer = ValuePtr;
    /// @private
    using reference = ValuePtr;

public:
    /// Create an empty iterator that does not point to an array.
    ///
    ValueIterator() noexcept = default;

private:
    /// Create an interator from a value.
    ///
    ValueIterator(ValuePtr value, std::size_t index) noexcept;

public: // operators
    /// Access the value.
    ///
    auto operator*() const noexcept -> reference;
    /// Access the value.
    ///
    auto operator->() noexcept -> pointer;

    /// Increment the iterator.
    ///
    auto operator++() noexcept -> ValueIterator&;

    /// Increment the iterator.
    ///
    auto operator++(int) noexcept -> ValueIterator;

    /// Compare two iterators.
    ///
    /// @param a The first iterator.
    /// @param b The second iterator.
    /// @return The result of the comparison.
    ///
    friend auto operator==(const ValueIterator& a, const ValueIterator& b) noexcept -> bool;
    /// @copydoc operator==(const ValueIterator&, const ValueIterator&)
    friend auto operator!=(const ValueIterator& a, const ValueIterator& b) noexcept -> bool;

private:
    std::size_t _index{}; ///< The current index in the array.
    ValuePtr _value{}; ///< The value that contains the array.
};


}

