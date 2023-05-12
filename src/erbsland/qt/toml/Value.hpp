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
#include "Location.hpp"

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
using ValuePtr = std::shared_ptr<Value>;


/// A value handled by the TOML parser or serializer.
///
/// @note This value type is not protected against infinite recursion. The parser will always produce valid results,
/// if a user is creating own value structure, they have to pay attention to this possibility.
///
class Value final {
    // fwd-entry: class Value
    // fwd-entry: using ValuePtr = std::shared_ptr<Value>

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
        std::unordered_map<QString, ValuePtr>, // 7, Table
        std::vector<ValuePtr>>;                // 8, Array

public:
    /// The type of a value.
    ///
    enum class Type {
        Integer, ///< A signed integer.
        Float, ///< A floating-point number.
        Boolean, ///< A boolean value (either `true` or `false`).
        String, ///< A string.
        Time, ///< A local time of day.
        Date, ///< A local calendar date.
        DateTime, ///< A date and time with or without offset.
        Table, ///< An unordered map of TOML key-value pairs.
        Array, ///< An ordered list of TOML values.
    };

    /// The source that defined the value.
    ///
    enum class Source {
        ImplicitTable, ///< Implicit key of a table `[this.this.key]`
        ExplicitTable, ///< Explicit key of a table `[key.key.this]`
        ImplicitValue, ///< Implicit key of a value `this.key.name = 5`
        ExplicitValue, ///< Explicit key of a value `key.this.name = 5`
        Value, ///< A value or an inline table or list: `key = { ... }` or `key = [ ... ]`
    };

public:
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

    /// Test if this is a table (`Table` or `InlineTable`).
    ///
    [[nodiscard]] inline auto isTable() const noexcept -> bool {
        return _type == Type::Table;
    }

    /// Test if this is an array (`Array` or `ArrayOfTables`).
    ///
    [[nodiscard]] inline auto isArray() const noexcept -> bool {
        return _type == Type::Array;
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

    /// Append a value to an array.
    ///
    /// If this value is no array, the call is ignored.
    ///
    /// @param value The value to add.
    ///
    void addValue(const ValuePtr &value) noexcept;

    /// Test if the value with a given key exists in a table.
    ///
    /// @param key The key to test.
    /// @return `true` if a value with that key exists, `false` otherwise.
    ///
    [[nodiscard]] auto hasValue(const QString &key) const noexcept -> bool;

    /// Access an value of a table.
    ///
    /// @param key The key.
    /// @return The value for the value, or a `nullptr` if the key does not exist.
    ///
    [[nodiscard]] auto value(const QString &key) const noexcept -> ValuePtr;

    /// Set or overwrite the value in a table.
    ///
    /// @param key The key of the value.
    /// @param value The value itself.
    ///
    void setValue(const QString &key, const ValuePtr &value) noexcept;

    /// Make this table explicit defined.
    ///
    void makeExplicit() noexcept;

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
    [[nodiscard]] auto toTable() const noexcept -> std::unordered_map<QString, ValuePtr>;

    /// Get an vector from this value.
    ///
    /// @return A vector if this value is `Type::Array`, otherwise an empty vector.
    ///
    [[nodiscard]] auto toArray() const noexcept -> std::vector<ValuePtr>;

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
    /// @internal
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
    template<std::size_t typeIndex>
    auto toValue() const noexcept -> std::variant_alternative_t<typeIndex, Storage>;

    /// A function that converts the type into the correct index for the storage.
    ///
    /// @note The compiler will optimize this function away and reduce it to a cast and check. It is safer than
    /// having an enum with integer type and do casts, as it never exceeds the valid range and causes compiler
    /// errors or warnings when the enum changes.
    ///
    /// @param type The type.
    /// @return The storage index.
    ///
    static constexpr auto typeToStorageIndex(Type type) noexcept -> std::size_t {
        switch (type) {
        case Type::Integer:
            return 0;
        case Type::Float:
            return 1;
        case Type::Boolean:
            return 2;
        case Type::String:
            return 3;
        case Type::Time:
            return 4;
        case Type::Date:
            return 5;
        case Type::DateTime:
            return 6;
        case Type::Table:
            return 7;
        case Type::Array:
            return 8;
        default:
            return 0;
        }
    }

private:
    Type _type; ///< The type for this value.
    Source _source; ///< The source for this value.
    Storage _storage; ///< The storage for this value.
};


}

