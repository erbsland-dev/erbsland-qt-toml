// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "ValueIterator.hpp"


#include "Value.hpp"

#include <variant>


namespace erbsland::qt::toml {


ValueIterator::ValueIterator(ValuePtr value, std::size_t index) noexcept
    : _value{std::move(value)}, _index{index} {
}


auto ValueIterator::operator*() const noexcept -> ValuePtr {
    if (_value == nullptr || !_value->isArray() || _index >= _value->size()) {
        return {};
    }
    return _value->value(_index);
}


auto ValueIterator::operator->() noexcept -> ValueIterator::pointer {
    if (_value == nullptr || !_value->isArray() || _index >= _value->size()) {
        return {};
    }
    return _value->value(_index);
}


auto ValueIterator::operator++() noexcept -> ValueIterator& {
    if (_value != nullptr && _value->isArray() && _index < _value->size()) {
        _index++;
    }
    return *this;
}


auto ValueIterator::operator++(int) noexcept -> ValueIterator {
    if (_value != nullptr && _value->isArray() && _index < _value->size()) {
        ValueIterator tmp = *this;
        _index++;
        return tmp;
    }
    return *this;
}


auto operator==(const ValueIterator &a, const ValueIterator &b) noexcept -> bool {
    return a._index == b._index && a._value == b._value;
}


auto operator!=(const ValueIterator &a, const ValueIterator &b) noexcept -> bool {
    return !operator==(a, b);
}


}


