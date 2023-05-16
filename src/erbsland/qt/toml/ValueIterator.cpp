// Copyright © 2023 Tobias Erbsland. Web: https://erbsland.dev
// Copyright © 2023 EducateIT GmbH. Web: https://educateit.ch
//
// This program is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with this program.
// If not, see <https://www.gnu.org/licenses/>.
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


