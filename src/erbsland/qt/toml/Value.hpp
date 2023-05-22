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
#pragma once


#include "Namespace.hpp"
#include "LocationRange.hpp"
#include "ValueIterator.hpp"
#include "ValueSource.hpp"
#include "ValueType.hpp"

#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtCore/QDate>
#include <QtCore/QDateTime>

#include <memory>
#include <variant>
#include <cstdint>
#include <unordered_map>


class QJsonValue;
class QVariant;


namespace erbsland::qt::toml {


class Value;
using ValuePtr = std::shared_ptr<Value>; ///< A shared pointer for the `Value` class.


/// A value handled by the TOML parser or serializer.
///
/// @note This value type is not protected against infinite recursion. The parser will always produce valid results,
/// if a user is creating own value structure, they have to pay attention to this possibility.
///
class Value final : public std::enable_shared_from_this<Value> {
    // fwd-entry: class Value
    friend class ValueIterator;

public:
    using TableValue = std::unordered_map<QString, ValuePtr>; ///< The storage type used for table values.
    using ArrayValue = std::vector<ValuePtr>; ///< The storage type used for arrays.

private:
    /// The variant used to store the values.
    ///
    using Storage = std::variant<
        int64_t,     // 0, Integer
        double,      // 1, Float
        bool,        // 2, Boolean
        QString,     // 3, String
        QTime,       // 4, Time
        QDate,       // 5, Date
        QDateTime,   // 6, DateTime
        TableValue,  // 7, Table
        ArrayValue>; // 8, Array

public: // local enum names.
    using Type = ValueType; ///< A local name for the value type enumeration.
    using Source = ValueSource; ///< A local name for the value source enumeration.

public: // access
    /// Get the type of this value.
    ///
    [[nodiscard]] inline auto type() const noexcept -> Type {
        return _type;
    }

    /// Get the source of this value.
    ///
    [[nodiscard]] inline auto source() const noexcept -> Source {
        return _source;
    }

    /// Get the location range.
    ///
    [[nodiscard]] inline auto locationRange() const noexcept -> LocationRange {
        return _locationRange;
    }

    /// Get the size of a table or array.
    ///
    /// @return The size of the table or array, or the size 0 for regular values.
    ///
    [[nodiscard]] auto size() const noexcept -> std::size_t;

    /// Access an value of an array.
    ///
    /// @param index The value index, starting from zero.
    /// @return The value for the value, or a `nullptr` if the index is out of bounds.
    ///
    [[nodiscard]] auto value(std::size_t index) const noexcept -> ValuePtr;

    /// Test if the value with a given key or key path exists in a table.
    ///
    /// @param keyPath The key, or a key path in the form `key.key.key`.
    /// @return `true` if a value with that key or key path exists, `false` otherwise.
    ///
    [[nodiscard]] auto hasValue(const QString &keyPath) const noexcept -> bool;

    /// Access a value of a table using a key or key path.
    ///
    /// @param keyPath The key, or a key path in the form `key.key.key`.
    /// @return The value for the value, or a `nullptr` if the key does not exist.
    ///
    [[nodiscard]] auto value(const QString &keyPath) const noexcept -> ValuePtr;

    /// Test if this table has a given key.
    ///
    /// This method exists, if you have to work with keys that contain a dot so you can't use
    /// the `hasValue()` methods with key paths.
    ///
    [[nodiscard]] auto hasKey(const QString &key) const noexcept -> bool;

    /// Access a value of this table, using a single key.
    ///
    /// This method exists, if you have to work with keys that contain a dot so you can't use
    /// the `value()` methods with key paths.
    ///
    /// @param key A single key, that can contain the dot character.
    /// @return The value for the value, or a `nullptr` if the key does not exist.
    ///
    [[nodiscard]] auto valueFromKey(const QString &key) const noexcept -> ValuePtr;

public: // convenience access
    /// Access a string value using a key path.
    ///
    /// @param keyPath The key path to the value, each key seperated with a dot. Like `key.key.key`.
    /// @param defaultValue The default value that is used if the key does not exist or is no string.
    /// @return The string at the given key path, or the `defaultValue`.
    ///
    [[nodiscard]] auto stringValue(const QString &keyPath, const QString &defaultValue = {}) const noexcept -> QString;

    /// Access an integer value using a key path.
    ///
    /// @param keyPath The key path to the value, each key separated with a dot. Like `key.key.key`.
    /// @param defaultValue The default value that is used if the key does not exist or is no integer.
    /// @return The integer at the given key path, or the `defaultValue`.
    ///
    [[nodiscard]] auto integerValue(const QString &keyPath, int64_t defaultValue = {}) const noexcept -> int64_t;

    /// Access a float value using a key path.
    ///
    /// @param keyPath The key path to the value, each key separated with a dot. Like `key.key.key`.
    /// @param defaultValue The default value that is used if the key does not exist or is no float.
    /// @return The float at the given key path, or the `defaultValue`.
    ///
    [[nodiscard]] auto floatValue(const QString &keyPath, double defaultValue = {}) const noexcept -> double;

    /// Access a boolean value using a key path.
    ///
    /// @param keyPath The key path to the value, each key separated with a dot. Like `key.key.key`.
    /// @param defaultValue The default value that is used if the key does not exist or is no boolean.
    /// @return The boolean at the given key path, or the `defaultValue`.
    ///
    [[nodiscard]] auto booleanValue(const QString &keyPath, bool defaultValue = {}) const noexcept -> bool;

    /// Access a time value using a key path.
    ///
    /// @param keyPath The key path to the value, each key separated with a dot. Like `key.key.key`.
    /// @param defaultValue The default value that is used if the key does not exist or is no time value.
    /// @return The time value at the given key path, or the `defaultValue`.
    ///
    [[nodiscard]] auto timeValue(const QString &keyPath, QTime defaultValue = {}) const noexcept -> QTime;

    /// Access a date value using a key path.
    ///
    /// @param keyPath The key path to the value, each key separated with a dot. Like `key.key.key`.
    /// @param defaultValue The default value that is used if the key does not exist or is no date value.
    /// @return The date value at the given key path, or the `defaultValue`.
    ///
    [[nodiscard]] auto dateValue(const QString &keyPath, QDate defaultValue = {}) const noexcept -> QDate;

    /// Access an date/time value using a key path.
    ///
    /// @param keyPath The key path to the value, each key separated with a dot. Like `key.key.key`.
    /// @param defaultValue The default value that is used if the key does not exist or is no date/time.
    /// @return The date/time at the given key path, or the `defaultValue`.
    ///
    [[nodiscard]] auto dateTimeValue(const QString &keyPath, const QDateTime& defaultValue = {}) const noexcept -> QDateTime;

    /// Access a table value using a key path.
    ///
    /// @param keyPath The key path to the value, each key separated with a dot. Like `key.key.key`.
    /// @return The table value at the given key path, or an empty unconnected table value.
    ///
    [[nodiscard]] auto tableValue(const QString &keyPath) const noexcept -> ValuePtr;

    /// Access an array value using a key path.
    ///
    /// @param keyPath The key path to the value, each key separated with a dot. Like `key.key.key`.
    /// @return The array value at the given key path, or an empty unconnected array value.
    ///
    [[nodiscard]] auto arrayValue(const QString &keyPath) const noexcept -> ValuePtr;

    /// Get a list with all keys of a table.
    ///
    /// @return An unsorted list with all keys in this table, or an empty list if this table is empty or
    ///    this value is no table.
    ///
    [[nodiscard]] auto tableKeys() const noexcept -> QStringList;

    /// Iterator over elements of an array.
    ///
    [[nodiscard]] auto begin() noexcept -> ValueIterator;

    /// Iterator over elements of an array.
    ///
    [[nodiscard]] auto end() noexcept -> ValueIterator;

public: // modification
    /// Set the location range.
    ///
    void setLocationRange(const LocationRange &locationRange) noexcept;

    /// Set or overwrite the value in a table.
    ///
    /// @param key The key of the value.
    /// @param value The value itself.
    ///
    void setValue(const QString &key, const ValuePtr &value) noexcept;

    /// Append a value to an array.
    ///
    /// If this value is no array, the call is ignored.
    ///
    /// @param value The value to add.
    ///
    void addValue(const ValuePtr &value) noexcept;

    /// Make this table explicit defined.
    ///
    void makeExplicit() noexcept;

    /// Deep-clone this value.
    ///
    /// @return A deep clone of this value and value structure if there is any.
    ///
    auto clone() const noexcept -> ValuePtr;

public: // tests
    /// Test if this is a table (`ValueType::Table`).
    ///
    [[nodiscard]] inline auto isTable() const noexcept -> bool {
        return _type == Type::Table;
    }

    /// Test if this is an array (`ValueType::Array`).
    ///
    [[nodiscard]] inline auto isArray() const noexcept -> bool {
        return _type == Type::Array;
    }

public: // conversion
    /// Get an integer from this value.
    ///
    /// @return The integer, if this value is of the `Type::Integer`, otherwise the value 0.
    ///
    [[nodiscard]] auto toInteger() const noexcept -> int64_t;

    /// Get an float from this value.
    ///
    /// @return The float, if this value is of the `Type::Float`, otherwise the value 0.0.
    ///
    [[nodiscard]] auto toFloat() const noexcept -> double;

    /// Get a boolean from this value.
    ///
    /// @return The boolean, if this value is of the `Type::Boolean`, otherwise the value false.
    ///
    [[nodiscard]] auto toBoolean() const noexcept -> bool;

    /// Get an string from this value.
    ///
    /// @return The string, if this value is of the `Type::String`, otherwise an empty string.
    ///
    [[nodiscard]] auto toString() const noexcept -> QString;

    /// Get a time from this value.
    ///
    /// @return The time, if this value is of the `Type::Time`, otherwise QTime{}.
    ///
    [[nodiscard]] auto toTime() const noexcept -> QTime;

    /// Get a date from this value.
    ///
    /// @return The date, if this value is of the `Type::Date`, otherwise QDate{}.
    ///
    [[nodiscard]] auto toDate() const noexcept -> QDate;

    /// Get a date/time from this value.
    ///
    /// @return The date/time, if this value is of the `Type::DateTime`, otherwise QDateTime{}.
    ///
    [[nodiscard]] auto toDateTime() const noexcept -> QDateTime;

    /// Get an unordered map from this value.
    ///
    /// @return An unordered map if this value is `Type::Table`, otherwise an empty map.
    ///
    [[nodiscard]] auto toTable() const noexcept -> TableValue;

    /// Get an vector from this value.
    ///
    /// @return A vector if this value is `Type::Array`, otherwise an empty vector.
    ///
    [[nodiscard]] auto toArray() const noexcept -> ArrayValue;

    /// Convert this value to a matching QJsonValue
    ///
    /// All structures are converted as expected by the TOML specification. Values that cannot be represented in
    /// JSON are converted into a string (time, date, date/time) that could be parsed by a TOML parser.
    ///
    /// @return This value as `QJsonValue`.
    ///
    [[nodiscard]] auto toJson() const noexcept -> QJsonValue;

    /// Convert this value to a QVariant
    ///
    /// Tables are converted into `QVariantHash`, arrays into `QVariantList`, all other values are converted
    /// into the best matching `QVariant` value.
    ///
    /// @return This value as `QVariant`.
    ///
    [[nodiscard]] auto toVariant() const noexcept -> QVariant;

    /// Convert this value to a QJsonValue for `toml-test`.
    ///
    /// This is a very specialized method to convert this value into the format expected by `toml-test`.
    /// It is part of the interface, as it can be useful to verify a TOML document. Structures with tables and
    /// arrays are converted like `toJson()`, but every other value creates an extra object with two entries
    /// `type` and `value`. The type is one of `string`, `integer`, `float`, `bool`, `datetime`, `datetime-local`
    /// `date-local` and `time-local` and the value is the TOML value as string.
    /// Please note that the string is recreated from the stored value. It is not the exact string as read
    /// from the document.
    ///
    /// @return The value in the special `toml-test` format as `QJsonValue`.
    ///
    [[nodiscard]] auto toUnitTestJson() const noexcept -> QJsonValue;

public:
    /// Create a new integer value.
    ///
    /// @param value The value of the new integer.
    /// @return A shared pointer to the new integer value.
    ///
    static auto createInteger(int64_t value) noexcept -> ValuePtr;

    /// Create a new floating-point value.
    ///
    /// @param value The value of the new floating-point number.
    /// @return A shared pointer to the new floating-point value.
    ///
    static auto createFloat(double value) noexcept -> ValuePtr;

    /// Create a new boolean value.
    ///
    /// @param value The value of the new boolean.
    /// @return A shared pointer to the new boolean value.
    ///
    static auto createBoolean(bool value) noexcept -> ValuePtr;

    /// Create a new string value.
    ///
    /// @param value The value of the new string.
    /// @return A shared pointer to the new string value.
    ///
    static auto createString(QString value) noexcept -> ValuePtr;

    /// Create a new time value.
    ///
    /// @param value The value of the new time.
    /// @return A shared pointer to the new time value.
    ///
    static auto createTime(QTime value) noexcept -> ValuePtr;

    /// Create a new date value.
    ///
    /// @param value The value of the new date.
    /// @return A shared pointer to the new date value.
    ///
    static auto createDate(QDate value) noexcept -> ValuePtr;

    /// Create a new date and time value.
    ///
    /// @param value The value of the new date and time.
    /// @return A shared pointer to the new date and time value.
    ///
    static auto createDateTime(QDateTime value) noexcept -> ValuePtr;

    /// Create a new empty table value.
    ///
    /// @param source The source of this table.
    /// @return A shared pointer to the new table value.
    ///
    static auto createTable(Source source) noexcept -> ValuePtr;

    /// Create a new empty array value.
    ///
    /// @param source The source of this array.
    /// @return A shared pointer to the new array value.
    ///
    static auto createArray(Source source) noexcept -> ValuePtr;

private:
    /// A tag for the private constructor.
    ///
    struct PrivateTag {};

public:
    /// @private
    /// The private constructor.
    ///
    /// @param type The value type
    /// @param source The source for the value.
    /// @param value The value.
    ///
    inline Value(Type type, Source source, Storage value, Value::PrivateTag /*unused*/) noexcept
        : _type{type}, _source{source}, _storage{std::move(value)} {
    }

private:
    /// Get the given type or the default value.
    ///
    template<typename T>
    auto toValue(Type type) const noexcept -> T;

    /// Return the value at the given key path if it exists and if it matches the type.
    ///
    /// @tparam Type The expected type.
    /// @param type The expected type enum.
    /// @param keyPath The key path in the form `key.key.key`
    /// @param defaultValue The default value returned if the key is not found or the type does not match.
    /// @return The value or `defaultValue`.
    ///
    template<typename T>
    auto typeValue(Type type, const QString &keyPath, const T &defaultValue) const noexcept -> T;

private:
    Type _type; ///< The type for this value.
    Source _source; ///< The source for this value.
    LocationRange _locationRange{LocationRange::createNotSet()}; ///< The location range for this value.
    Storage _storage; ///< The storage for this value.
};


}

