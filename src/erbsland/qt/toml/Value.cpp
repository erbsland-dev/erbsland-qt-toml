// Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch
// According to the copyright terms specified in the file "COPYRIGHT.md".
// SPDX-License-Identifier: LGPL-3.0-or-later
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
    if (auto ptr = std::get_if<TableValue>(&_storage); ptr != nullptr) {
        return ptr->size();
    }
    if (auto ptr = std::get_if<ArrayValue>(&_storage); ptr != nullptr) {
        return ptr->size();
    }
    return 0;
}


auto Value::value(std::size_t index) const noexcept -> ValuePtr {
    if (!isArray()) {
        return {};
    }
    if (auto ptr = std::get_if<ArrayValue>(&_storage); ptr != nullptr) {
        if (index < ptr->size()) {
            return ptr->at(index);
        }
    }
    return {};
}


auto Value::hasValue(const QString &keyPath) const noexcept -> bool {
    return value(keyPath) != nullptr;
}


auto Value::hasKey(const QString &key) const noexcept -> bool {
    return valueFromKey(key) != nullptr;
}


auto Value::value(const QString &keyPath) const noexcept -> ValuePtr {
    if (!isTable()) {
        return {};
    }
    if (!keyPath.contains('.')) { // single key?
        return valueFromKey(keyPath);
    }
    auto frontKey = keyPath.section('.', 0, 0);
    auto backKeys = keyPath.section('.', 1);
    if (auto ptr = std::get_if<TableValue>(&_storage); ptr != nullptr) {
        auto it = ptr->find(frontKey);
        if (it != ptr->end()) {
            return it->second->value(backKeys);
        }
    }
    return {};
}


auto Value::valueFromKey(const QString &key) const noexcept -> ValuePtr {
    if (!isTable()) {
        return {};
    }
    if (auto ptr = std::get_if<TableValue>(&_storage); ptr != nullptr) {
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
    if (auto ptr = std::get_if<ArrayValue>(&_storage); ptr != nullptr) {
        ptr->emplace_back(value);
    }
}


void Value::setValue(const QString &key, const ValuePtr &value) noexcept {
    if (!isTable()) {
        return;
    }
    if (auto ptr = std::get_if<TableValue>(&_storage); ptr != nullptr) {
        ptr->insert_or_assign(key, value);
    }
}


auto Value::tableKeys() const noexcept -> QStringList {
    if (!isTable()) {
        return {};
    }
    QStringList result;
    if (auto ptr = std::get_if<TableValue>(&_storage); ptr != nullptr) {
        for (const auto &entry : *ptr) {
            result.append(entry.first);
        }
    }
    return result;
}


template<typename T>
auto Value::toValue(Type type) const noexcept -> T {
    if (_type != type) {
        return {};
    }
    return std::get<T>(_storage);
}


auto Value::toInteger() const noexcept -> int64_t {
    return toValue<int64_t>(Type::Integer);
}


auto Value::toFloat() const noexcept -> double {
    return toValue<double>(Type::Float);
}


auto Value::toBoolean() const noexcept -> bool {
    return toValue<bool>(Type::Boolean);
}


auto Value::toString() const noexcept -> QString {
    return toValue<QString>(Type::String);
}


auto Value::toTime() const noexcept -> QTime {
    return toValue<QTime>(Type::Time);
}


auto Value::toDate() const noexcept -> QDate {
    return toValue<QDate>(Type::Date);
}


auto Value::toDateTime() const noexcept -> QDateTime {
    return toValue<QDateTime>(Type::DateTime);
}


auto Value::toTable() const noexcept -> TableValue {
    return toValue<TableValue>(Type::Table);
}


auto Value::toArray() const noexcept -> ArrayValue {
    return toValue<ArrayValue>(Type::Array);
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
    return std::make_shared<Value>(Type::Table, source, Storage{TableValue{}}, PrivateTag{});
}


auto Value::createArray(Source source) noexcept -> ValuePtr {
    return std::make_shared<Value>(Type::Array, source, Storage{ArrayValue{}}, PrivateTag{});
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
    if (type() == Type::Table) {
        QVariantMap variantMap;
        for (const auto &[key, value] : toTable()) {
            variantMap.insert(key, value->toVariant());
        }
        return variantMap;
    } else if (type() == Type::Array) {
        QVariantList variantList;
        for (const auto &value: toArray()) {
            variantList.append(value->toVariant());
        }
        return variantList;
    }

    switch (type()) {
        case Type::Integer:
            return toInteger();
        case Type::Float:
            return toFloat();
        case Type::Boolean:
            return toBoolean();
        case Type::String:
            return toString();
        case Type::Time:
            return toTime();
        case Type::Date:
            return toDate();
        case Type::DateTime:
            return toDateTime();
        default:
            return {};
    }
}


auto Value::toUnitTestJson() const noexcept -> QJsonValue {
    if (type() == Type::Table) {
        QJsonObject jsonObject;
        for (const auto &[key, value] : toTable()) {
            jsonObject[key] = value->toUnitTestJson();
        }
        return jsonObject;
    }

    if (type() == Type::Array) {
        QJsonArray jsonArray;
        for (const auto &value: toArray()) {
            jsonArray.append(value->toUnitTestJson());
        }
        return jsonArray;
    }

    QString typeStr = valueTypeToUnitTestString(type());
    QString valueStr;

    switch (type()) {
        case Type::Integer:
            valueStr = QString::number(toInteger());
            break;
        case Type::Float: {
            auto value = toFloat();
            valueStr = std::isnan(value) ? QStringLiteral("nan") : QString::number(toFloat(), 'g', 20);
            break;
        }
        case Type::Boolean:
            valueStr = toBoolean() ? QStringLiteral("true") : QStringLiteral("false");
            break;
        case Type::String:
            valueStr = toString();
            break;
        case Type::Time:
            valueStr = toTime().toString(Qt::ISODateWithMs);
            break;
        case Type::Date:
            valueStr = toDate().toString(Qt::ISODate);
            break;
        case Type::DateTime: {
            auto dateTime = toDateTime();
            if (dateTime.timeSpec() == Qt::LocalTime) {
                typeStr = QStringLiteral("datetime-local");
            }
            valueStr = dateTime.toString(Qt::ISODateWithMs);
            break;
        }
        default:
            break;
    }

    QJsonObject valueObj;
    valueObj["type"] = typeStr;
    valueObj["value"] = valueStr;
    return valueObj;
}


template<typename T>
auto Value::typeValue(ValueType type, const QString &keyPath, const T &defaultValue) const noexcept -> T {
    auto tableValue = value(keyPath);
    if (tableValue == nullptr || tableValue->type() != type) {
        return defaultValue;
    }
    return std::get<T>(tableValue->_storage);
}


auto Value::stringValue(const QString &keyPath, const QString &defaultValue) const noexcept -> QString {
    return typeValue<QString>(Type::String, keyPath, defaultValue);
}


auto Value::integerValue(const QString &keyPath, int64_t defaultValue) const noexcept -> int64_t {
    return typeValue<int64_t>(Type::Integer, keyPath, defaultValue);
}


auto Value::floatValue(const QString &keyPath, double defaultValue) const noexcept -> double {
    return typeValue<double>(Type::Float, keyPath, defaultValue);
}


auto Value::booleanValue(const QString &keyPath, bool defaultValue) const noexcept -> bool {
    return typeValue<bool>(Type::Boolean, keyPath, defaultValue);
}


auto Value::timeValue(const QString &keyPath, QTime defaultValue) const noexcept -> QTime {
    return typeValue<QTime>(Type::Time, keyPath, defaultValue);
}


auto Value::dateValue(const QString &keyPath, QDate defaultValue) const noexcept -> QDate {
    return typeValue<QDate>(Type::Date, keyPath, defaultValue);
}


auto Value::dateTimeValue(const QString &keyPath, const QDateTime& defaultValue) const noexcept -> QDateTime {
    return typeValue<QDateTime>(Type::DateTime, keyPath, defaultValue);
}


auto Value::tableValue(const QString &keyPath) const noexcept -> ValuePtr {
    auto value = this->value(keyPath);
    if (value == nullptr || !value->isTable()) {
        return createTable(Source::Value);
    }
    return value;
}


auto Value::arrayValue(const QString &keyPath) const noexcept -> ValuePtr {
    auto value = this->value(keyPath);
    if (value == nullptr || !value->isArray()) {
        return createArray(Source::Value);
    }
    return value;
}


auto Value::clone() const noexcept -> ValuePtr {
    ValuePtr newValue;
    if (isTable()) {
        newValue = createTable(source());
        for (const auto &[key, value] : toTable()) {
            newValue->setValue(key, value->clone());
        }
    } else if (isArray()) {
        newValue = createArray(source());
        for (const auto &value : toArray()) {
            newValue->addValue(value->clone());
        }
    } else {
        newValue = std::make_shared<Value>(_type, _source, _storage, PrivateTag{});
    }
    newValue->setLocationRange(_locationRange);
    return newValue;
}


auto Value::begin() noexcept -> ValueIterator {
    if (!isArray()) {
        return {};
    }
    return {shared_from_this(), 0};
}


auto Value::end() noexcept -> ValueIterator {
    if (!isArray()) {
        return {};
    }
    return {shared_from_this(), size()};
}


void Value::setLocationRange(const LocationRange &locationRange) noexcept {
    _locationRange = locationRange;
}


}

