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
#include "Value.hpp"


#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>

#include <utility>
#include <exception>


namespace erbsland::qt::toml {


auto Value::size() const noexcept -> std::size_t {
    if (!isTable() && !isArray()) {
        return 0;
    }
    if (auto ptr = std::get_if<typeToStorageIndex(Type::Table)>(&_storage); ptr != nullptr) {
        return ptr->size();
    }
    if (auto ptr = std::get_if<typeToStorageIndex(Type::Array)>(&_storage); ptr != nullptr) {
        return ptr->size();
    }
    return 0;
}


auto Value::value(std::size_t index) const noexcept -> ValuePtr {
    if (!isArray()) {
        return {};
    }
    if (auto ptr = std::get_if<typeToStorageIndex(Type::Array)>(&_storage); ptr != nullptr) {
        if (index < ptr->size()) {
            return ptr->at(index);
        }
    }
    return {};
}


auto Value::hasValue(const QString &key) const noexcept -> bool {
    if (!isTable()) {
        return false;
    }
    if (auto ptr = std::get_if<typeToStorageIndex(Type::Table)>(&_storage); ptr != nullptr) {
        return ptr->find(key) != ptr->end();
    }
    return false;
}


auto Value::value(const QString &key) const noexcept -> ValuePtr {
    if (!isTable()) {
        return {};
    }
    if (auto ptr = std::get_if<typeToStorageIndex(Type::Table)>(&_storage); ptr != nullptr) {
        auto it = ptr->find(key);
        if (it != ptr->end()) {
            return it->second;
        }
    }
    return {};
}


void Value::addValue(const ValuePtr &value) noexcept {
    if (!isArray()) {
        return;
    }
    if (_source == Source::ExplicitTable && !value->isTable()) {
        return;
    }
    if (auto ptr = std::get_if<typeToStorageIndex(Type::Array)>(&_storage); ptr != nullptr) {
        ptr->emplace_back(value);
    }
}


void Value::setValue(const QString &key, const ValuePtr &value) noexcept {
    if (!isTable()) {
        return;
    }
    if (auto ptr = std::get_if<typeToStorageIndex(Type::Table)>(&_storage); ptr != nullptr) {
        ptr->insert_or_assign(key, value);
    }
}


template<std::size_t typeIndex>
auto Value::toValue() const noexcept -> std::variant_alternative_t<typeIndex, Storage> {
    if (_storage.index() == typeIndex) {
        return std::get<typeIndex>(_storage);
    }
    return std::variant_alternative_t<typeIndex, Storage>{};
}


auto Value::toInteger() const noexcept -> int64_t {
    return toValue<typeToStorageIndex(Type::Integer)>();
}


auto Value::toFloat() const noexcept -> double {
    return toValue<typeToStorageIndex(Type::Float)>();
}


auto Value::toBoolean() const noexcept -> bool {
    return toValue<typeToStorageIndex(Type::Boolean)>();
}


auto Value::toString() const noexcept -> QString {
    return toValue<typeToStorageIndex(Type::String)>();
}


auto Value::toTime() const noexcept -> QTime {
    return toValue<typeToStorageIndex(Type::Time)>();
}


auto Value::toDate() const noexcept -> QDate {
    return toValue<typeToStorageIndex(Type::Date)>();
}


auto Value::toDateTime() const noexcept -> QDateTime {
    return toValue<typeToStorageIndex(Type::DateTime)>();
}


auto Value::toTable() const noexcept -> std::unordered_map<QString, ValuePtr> {
    return toValue<typeToStorageIndex(Type::Table)>();
}


auto Value::toArray() const noexcept -> std::vector<ValuePtr> {
    return toValue<typeToStorageIndex(Type::Array)>();
}


auto Value::createInteger(int64_t value) noexcept -> ValuePtr {
    return std::make_shared<Value>(Type::Integer, Source::Value, Storage{value}, PrivateTag{});
}


auto Value::createFloat(double value) noexcept -> ValuePtr {
    return std::make_shared<Value>(Type::Float, Source::Value, Storage{value}, PrivateTag{});
}


auto Value::createBoolean(bool value) noexcept -> ValuePtr {
    return std::make_shared<Value>(Type::Boolean, Source::Value, Storage{value}, PrivateTag{});
}


auto Value::createString(QString value) noexcept -> ValuePtr {
    return std::make_shared<Value>(Type::String, Source::Value, Storage{std::move(value)}, PrivateTag{});
}


auto Value::createTime(QTime value) noexcept -> ValuePtr {
    return std::make_shared<Value>(Type::Time, Source::Value, Storage{value}, PrivateTag{});
}


auto Value::createDate(QDate value) noexcept -> ValuePtr {
    return std::make_shared<Value>(Type::Date, Source::Value, Storage{value}, PrivateTag{});
}


auto Value::createDateTime(QDateTime value) noexcept -> ValuePtr {
    return std::make_shared<Value>(Type::DateTime, Source::Value, Storage{std::move(value)}, PrivateTag{});
}


auto Value::createTable(Source source) noexcept -> ValuePtr {
    return std::make_shared<Value>(Type::Table, source, Storage{std::unordered_map<QString, ValuePtr>{}}, PrivateTag{});
}


auto Value::createArray(Source source) noexcept -> ValuePtr {
    return std::make_shared<Value>(Type::Array, source, Storage{std::vector<ValuePtr>{}}, PrivateTag{});
}


void Value::makeExplicit() noexcept {
    if (_source == Source::ImplicitTable) {
        _source = Source::ExplicitTable;
    } else if (_source == Source::ImplicitValue) {
        _source = Source::ExplicitValue;
    }
}


auto Value::toJson() const noexcept -> QJsonValue {
    switch (type()) {
    case Type::Integer:
        return QJsonValue{toInteger()};
    case Type::Float:
        return QJsonValue{toFloat()};
    case Type::Boolean:
        return QJsonValue{toBoolean()};
    case Type::String:
        return QJsonValue{toString()};
    case Type::Time:
        return QJsonValue{toTime().toString(Qt::ISODateWithMs)};
    case Type::Date:
        return QJsonValue{toDate().toString(Qt::ISODateWithMs)};
    case Type::DateTime:
        return QJsonValue{toDateTime().toString(Qt::ISODateWithMs)};
    case Type::Table: {
        QJsonObject jsonObject;
        for (const auto &[key, value] : toTable()) {
            jsonObject[key] = value->toJson();
        }
        return jsonObject;
    }
    case Type::Array: {
        QJsonArray jsonArray;
        for (const auto &value: toArray()) {
            jsonArray.append(value->toJson());
        }
        return jsonArray;
    }
    default:
        return {};
    }
}


auto Value::toVariant() const noexcept -> QVariant {
    switch (type()) {
    case Type::Integer:
        return QVariant{toInteger()};
    case Type::Float:
        return QVariant{toFloat()};
    case Type::Boolean:
        return QVariant{toBoolean()};
    case Type::String:
        return QVariant{toString()};
    case Type::Time:
        return QVariant{toTime()};
    case Type::Date:
        return QVariant{toDate()};
    case Type::DateTime:
        return QVariant{toDateTime()};
    case Type::Table: {
        QVariantMap variantMap;
        for (const auto &[key, value] : toTable()) {
            variantMap.insert(key, value->toVariant());
        }
        return variantMap;
    }
    case Type::Array: {
        QVariantList variantList;
        for (const auto &value: toArray()) {
            variantList.append(value->toVariant());
        }
        return variantList;
    }
    default:
        return {};
    }
}


auto Value::toUnitTestJson() const noexcept -> QJsonValue {
    switch (type()) {
    case Type::Integer: {
        QJsonObject valueObj;
        valueObj["type"] = QStringLiteral("integer");
        valueObj["value"] = QString::number(toInteger());
        return valueObj;
    }
    case Type::Float: {
        QJsonObject valueObj;
        valueObj["type"] = QStringLiteral("float");
        auto value = toFloat();
        if (std::isnan(value)) {
            valueObj["value"] = QStringLiteral("nan");
        } else {
            valueObj["value"] = QString::number(toFloat(), 'g', 20);
        }
        return valueObj;
    }
    case Type::Boolean: {
        QJsonObject valueObj;
        valueObj["type"] = QStringLiteral("bool");
        valueObj["value"] = toBoolean() ? QStringLiteral("true") : QStringLiteral("false");
        return valueObj;
    }
    case Type::String: {
        QJsonObject valueObj;
        valueObj["type"] = QStringLiteral("string");
        valueObj["value"] = toString();
        return valueObj;
    }
    case Type::Time: {
        QJsonObject valueObj;
        valueObj["type"] = QStringLiteral("time-local");
        valueObj["value"] = toTime().toString(Qt::ISODateWithMs);
        return valueObj;
    }
    case Type::Date: {
        QJsonObject valueObj;
        valueObj["type"] = QStringLiteral("date-local");
        valueObj["value"] = toDate().toString(Qt::ISODate);
        return valueObj;
    }
    case Type::DateTime: {
        QJsonObject valueObj;
        auto dateTime = toDateTime();
        if (dateTime.timeSpec() == Qt::LocalTime) {
            valueObj["type"] = QStringLiteral("datetime-local");
        } else {
            valueObj["type"] = QStringLiteral("datetime");
        }
        valueObj["value"] = toDateTime().toString(Qt::ISODateWithMs);
        return valueObj;
    }
    case Type::Table: {
        QJsonObject jsonObject;
        for (const auto &[key, value] : toTable()) {
            jsonObject[key] = value->toUnitTestJson();
        }
        return jsonObject;
    }
    case Type::Array: {
        QJsonArray jsonArray;
        for (const auto &value: toArray()) {
            jsonArray.append(value->toUnitTestJson());
        }
        return jsonArray;
    }
    default:
        return {};
    }
}


}

