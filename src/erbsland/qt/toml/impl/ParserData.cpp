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
#include "ParserData.hpp"


#include "../Error.hpp"

#include <QtCore/QTime>
#include <QtCore/QDate>
#include <QtCore/QDateTime>


namespace erbsland::qt::toml::impl {


ParserData::ParserData(Specification specification) noexcept
    : _specification{specification}, _tokenizer{specification} {
}


auto ParserData::parseStream(const InputStreamPtr &inputStream) -> ValuePtr {
    try {
        _tokenizer.startWithStream(inputStream);
        parseDocument();
        _tokenizer.stop();
        return std::exchange(_document, {});
    } catch (std::exception&) {
        _tokenizer.stop();
        _document = {};
        throw;
    }
}


void ParserData::parseDocument() {
    // Create the root table and set it as current context.
    _document = Value::createTable(Value::Source::ExplicitTable);
    _currentTable = _document;
    readNextToken(); // next non whitespace/comment token.
    while (!_token.isEndOfDocument()) {
        if (_token.isNewLine()) { // Skip all newlines
            readNextToken();
        } else if (_token.isKey()) {
            parseDocumentLevelAssignment();
        } else if (_token.type() == TokenType::TableNameBegin) {
            parseTableName();
        } else if (_token.type() == TokenType::ArrayNameBegin) {
            parseArrayOfTablesName();
        } else {
            throwSyntaxError(QStringLiteral("Expected a table, array or assignment."));
        }
    }
}


void ParserData::parseDocumentLevelAssignment() {
    parseKeyValueAssignment();
    // after the value, there must be at least one newline or the end of the document.
    readNextToken();
    if (!_token.isNewLine() && !_token.isEndOfDocument()) {
        throwSyntaxError(QStringLiteral("Expected new-line after value."));
    }
}


void ParserData::parseKeyValueAssignment() {
    auto valuePath = std::vector<Token>{_token};
    readAndRequireNextToken();
    while (_token.isKeySeperator()) {
        readAndRequireNextToken();
        if (!_token.isKey()) {
            throwSyntaxError(QStringLiteral("Expected another key after the dot-seperator."));
        }
        valuePath.emplace_back(_token);
        readAndRequireNextToken();
    }
    if (_token.type() != TokenType::Assignment) {
        throwSyntaxError(QStringLiteral("Expected assignment operator after key."));
    }
    readAndRequireNextToken(); // expect a value token next
    auto value = parseValue(); // read the next token and assume we get a value.
    assignValue(valuePath, value);
}


void ParserData::parseTableName() {
    readAndRequireNextToken(); // Expect a name.
    if (!_token.isKey()) {
        throwSyntaxError(QStringLiteral("Expected a name after open table bracket."));
    }
    std::vector<Token> keys{_token};
    readAndRequireNextToken(); // Expect end of table name or name seperator
    while (_token.type() != TokenType::TableNameEnd) {
        if (!_token.isKeySeperator()) {
            throwSyntaxError(QStringLiteral("Expected a dot-seperator or the closing table bracket."));
        }
        readAndRequireNextToken();
        if (!_token.isKey()) {
            throwSyntaxError(QStringLiteral("Expected another name after the dot-seperator."));
        }
        keys.emplace_back(_token);
        readAndRequireNextToken();
    }
    createTable(keys);
    readNextToken();
    if (!_token.isNewLine() && !_token.isEndOfDocument()) {
        throwSyntaxError(QStringLiteral("Expected a new-line after the table name."));
    }
}


void ParserData::parseArrayOfTablesName() {
    readAndRequireNextToken(); // Expect a name.
    if (!_token.isKey()) {
        throwSyntaxError(QStringLiteral("Expected a name after open array bracket."));
    }
    std::vector<Token> keys{_token};
    readAndRequireNextToken(); // Expect end of array name or name seperator
    while (_token.type() != TokenType::ArrayNameEnd) {
        if (!_token.isKeySeperator()) {
            throwSyntaxError(QStringLiteral("Expected a dot-seperator or the closing array bracket."));
        }
        readAndRequireNextToken();
        if (!_token.isKey()) {
            throwSyntaxError(QStringLiteral("Expected another name after the dot-seperator."));
        }
        keys.emplace_back(_token);
        readAndRequireNextToken();
    }
    createArrayOfTables(keys);
    readNextToken();
    if (!_token.isNewLine() && !_token.isEndOfDocument()) {
        throwSyntaxError(QStringLiteral("Expected a new-line after the table name."));
    }
}


void ParserData::createTable(std::vector<Token> keys) {
    auto key = keys.back();
    keys.pop_back();
    auto table = createIntermediateNameElements(keys, _document, false);
    if (table->hasValue(key.text())) {
        auto value = table->value(key.text());
        if (!value->isTable()) {
            throwSyntaxError(QStringLiteral("The key already exists and is no table."), key);
        }
        if (value->source() == Value::Source::Value) {
            throwSyntaxError(QStringLiteral("The table with that key is an inline table."), key);
        }
        if (value->source() == Value::Source::ImplicitValue || value->source() == Value::Source::ExplicitValue) {
            throwSyntaxError(QStringLiteral("The table with that key was created by a dotted key of a value assignment."), key);
        }
        if (value->source() == Value::Source::ExplicitTable) {
            throwSyntaxError(QStringLiteral("The table with that key already exists."), key);
        }
        _currentTable = value;
        _currentTable->makeExplicit();
    } else {
        _currentTable = Value::createTable(Value::Source::ExplicitTable);
    }
    table->setValue(key.text(), _currentTable);
}


void ParserData::createArrayOfTables(std::vector<Token> keys) {
    auto key = keys.back();
    keys.pop_back();
    auto table = createIntermediateNameElements(keys, _document, false);
    if (table->hasValue(key.text())) {
        auto value = table->value(key.text());
        if (!value->isArray()) {
            throwSyntaxError(QStringLiteral("The key exists, but is no array."), key);
        }
        if (value->source() == Value::Source::Value) {
            throwSyntaxError(QStringLiteral("You can not extend a regular array with this syntax."), key);
        }
        // implicit and explicit tables should not exist.
        auto newTable = Value::createTable(Value::Source::ExplicitTable);
        value->addValue(newTable);
        _currentTable = newTable;
    } else {
        auto newArray = Value::createArray(Value::Source::ExplicitTable);
        table->setValue(key.text(), newArray);
        auto newTable = Value::createTable(Value::Source::ExplicitTable);
        newArray->addValue(newTable);
        _currentTable = newTable;
    }
}


auto ParserData::createIntermediateNameElements(
    const std::vector<Token>& keys,
    const ValuePtr &baseTable,
    bool isValueAssignment) -> ValuePtr {

    auto result = baseTable;
    for (const auto &key : keys) {
        if (result->hasValue(key.text())) {
            result = result->value(key.text());
            if (result->source() == Value::Source::Value) {
                throwSyntaxError(QStringLiteral("A dotted key must not point to an existing value."));
            }
            if (result->isArray()) { // must be an array of tables.
                if (isValueAssignment) {
                    throwSyntaxError(QStringLiteral("A dotted key of a value must not point to an array of tables."));
                }
                if (result->size() == 0) {
                    // An array of tables must have always at least one table element.
                    throw std::logic_error("A key points to an empty array of tables.");
                }
                // Get the last element from an array.
                result = result->value(result->size() - 1);
                if (!result->isTable()) {
                    // An array of tables must only contain table elements.
                    throw std::logic_error("A key points to an array of tables that contains not a table.");
                }
            } else {
                // As only arrays and table have a non `Value` source, must be a table.
                if (isValueAssignment && (result->source() == Value::Source::ImplicitTable || result->source() == Value::Source::ExplicitTable)) {
                    throwSyntaxError(QStringLiteral("A dotted key of a value must not point to explicitly defined tables."));
                }
            }
        } else {
            // If the key does not exist, create a new table for the value or structure.
            auto newTable = Value::createTable(
                isValueAssignment ? Value::Source::ImplicitValue : Value::Source::ImplicitTable);
            result->setValue(key.text(), newTable);
            result = newTable;
        }
    }
    return result;
}


auto ParserData::parseValue() -> ValuePtr {
    switch (_token.type()) {
    case TokenType::TableBegin:
        return parseInlineTableValue();
    case TokenType::ArrayBegin:
        return parseArrayValue();
    case TokenType::SingleLineString:
    case TokenType::MultiLineString:
        return Value::createString(_token.text());
    case TokenType::Boolean:
        return Value::createBoolean(_token.text() == "true");
    case TokenType::DecimalInteger:
        return parseIntegerValue();
    case TokenType::HexInteger:
        return Value::createInteger(QStringView{_token.text()}.mid(2).toLongLong(nullptr, 16));
    case TokenType::BinaryInteger:
        return Value::createInteger(QStringView{_token.text()}.mid(2).toLongLong(nullptr, 2));
    case TokenType::OctalInteger:
        return Value::createInteger(QStringView{_token.text()}.mid(2).toLongLong(nullptr, 8));
    case TokenType::Float:
        return parseFloatValue();
    case TokenType::OffsetDateTime:
    case TokenType::LocalDateTime:
        return parseDateTimeValue();
    case TokenType::LocalDate:
        return Value::createDate(QDate::fromString(_token.text(), Qt::ISODate));
    case TokenType::LocalTime:
        return parseTimeValue();
    default:
        throwSyntaxError(QStringLiteral("Expected a value after the assignment operator."));
    }
}


auto ParserData::parseIntegerValue() -> ValuePtr {
    // Make sure the integer does not start with a zero
    auto text = QStringView(_token.text());
    if (text.startsWith('+') || text.startsWith('-')) {
        text = text.mid(1);
    }
    if (text != QStringLiteral("0") && text.startsWith('0')) {
        throwSyntaxError(QStringLiteral("Leading zeros are not allowed for integer values."));
    }
    return Value::createInteger(_token.text().toLongLong());
}


auto ParserData::parseFloatValue() -> ValuePtr {
    // Make sure the float does not start with a zero.
    auto text = QStringView(_token.text());
    if (text.startsWith('+') || text.startsWith('-')) {
        text = text.mid(1);
    }
    if (text.compare(QStringLiteral("nan"), Qt::CaseInsensitive) == 0) {
        // as there is no negative nan and Qt does a poor job parsing it, create the nan manually.
        return Value::createFloat(std::numeric_limits<double>::quiet_NaN());
    }
    if (!(text.startsWith(QStringLiteral("0.")) || text.startsWith(QStringLiteral("0e"), Qt::CaseInsensitive))) {
        if (text.startsWith('0')) {
            throwSyntaxError(QStringLiteral("Leading zeros are not allowed for floating point values."));
        }
    }
    return Value::createFloat(_token.text().toDouble());
}


auto ParserData::parseTimeValue() -> ValuePtr {
    auto text = QStringView(_token.text());
    auto [time, timeSpec, offset] = convertTime(text);
    return Value::createTime(time);
}


auto ParserData::parseDateTimeValue() -> ValuePtr {
    auto text = QStringView(_token.text());
    auto date = convertDate(text.left(10));
    auto [time, timeSpec, offset] = convertTime(text.mid(11));
    auto dateTime = QDateTime{date, time, timeSpec, offset};
    return Value::createDateTime(dateTime);
}


auto ParserData::convertDate(const QStringView &text) -> QDate {
    auto date = QDate::fromString(text.toString(), Qt::ISODate);
    if (!date.isValid()) {
        throwSyntaxError(QStringLiteral("The date/time value is not valid. Invalid date."));
    }
    return date;
}


auto ParserData::convertTime(const QStringView &text) -> std::tuple<QTime, Qt::TimeSpec, int> {
    qsizetype offsetSeconds = 0;
    qsizetype offsetIndex = -1;
    bool hasOffset = false;
    int fractionMilliseconds = 0;
    if (text.endsWith('z', Qt::CaseInsensitive)) {
        hasOffset = true;
        offsetIndex = text.size() - 1;
    } else {
        offsetIndex = std::max(text.indexOf('-'), text.indexOf('+'));
        if (offsetIndex >= 0) {
            hasOffset = true;
            auto offsetHour = text.mid(offsetIndex + 1, 2).toInt();
            if (offsetHour >= 24) {
                throwSyntaxError(QStringLiteral("The time value is not valid. Offset hour is not valid."));
            }
            auto offsetMinute = text.mid(offsetIndex + 4, 2).toInt();
            if (offsetMinute >= 60) {
                throwSyntaxError(QStringLiteral("The time value is not valid. Offset minute is not valid."));
            }
            offsetSeconds = 3600 * offsetHour + 60 * offsetMinute;
            if (text[offsetIndex] == '-') {
                offsetSeconds *= -1;
            }
        }
    }
    auto fractionIndex = text.indexOf('.');
    if (fractionIndex >= 0) {
        qsizetype fractionLength = 3;
        if (offsetIndex >= 0) {
            fractionLength = std::min(offsetIndex - (fractionIndex + 1), static_cast<qsizetype>(3));
        }
        fractionMilliseconds = text.mid(fractionIndex + 1, fractionLength)
            .toString().leftJustified(3, QChar('0')).toInt();
    }
    auto timeLength = (fractionIndex >= 0) ? fractionIndex : ((offsetIndex >= 0) ? offsetIndex : text.size());
    auto timeParts = text.left(timeLength).split(':');
    auto hour = timeParts.value(0).toInt();
    auto minute = timeParts.value(1).toInt();
    auto second = timeParts.value(2).toInt();
    if (hour > 23) {
        throwSyntaxError(QStringLiteral("The time value is not valid. Hour exceeds 23."));
    }
    if (minute > 59) {
        throwSyntaxError(QStringLiteral("The time value is not valid. Minute exceeds 59."));
    }
    if (second > 59) {
        throwSyntaxError(QStringLiteral("The time value is not valid. Second exceeds 59."));
    }
    return std::make_tuple(
        QTime{hour, minute, second, fractionMilliseconds},
        !hasOffset ? Qt::LocalTime : ((offsetSeconds == 0) ? Qt::UTC : Qt::OffsetFromUTC),
        offsetSeconds);
}


auto ParserData::parseArrayValue() -> ValuePtr {
    auto array = Value::createArray(Value::Source::Value);
    readAndRequireNextToken(); // Expect a value or array end.
    while (_token.type() != TokenType::ArrayEnd) {
        if (_token.isNewLine()) {
            readAndRequireNextToken();
            continue; // Skip newlines in an array.
        }
        auto value = parseValue();
        array->addValue(value);
        readAndRequireNextToken(); // Expect a value separator, value, or array end.
        while (_token.isNewLine()) { // Skip any number of newlines after the value.
            readAndRequireNextToken();
        }
        if (_token.type() == TokenType::TableSeperator) {
            readAndRequireNextToken(); // Expect a value or end of array after the separator.
        } else if (_token.type() != TokenType::ArrayEnd) {
            throwSyntaxError(QStringLiteral("Expected a value separator or the end of the array."));
        }
    }
    return array;
}


auto ParserData::parseInlineTableValue() -> ValuePtr {
    auto table = Value::createTable(Value::Source::Value);
    readAndRequireNextToken(); // Expect a name or the end of the table.
    while (_token.type() != TokenType::TableEnd) {
        if (_token.isNewLine()) {
            if (_specification >= Specification::Version_1_1) {
                readAndRequireNextToken();
                continue; // Skip newlines for version 1.1
            }
            throwSyntaxError(QStringLiteral("Newlines are not allowed in inline tables for TOML 1.0."));
        }
        if (!_token.isKey()) {
            throwSyntaxError(QStringLiteral("Expected a key, but got something else."));
        }
        std::vector<Token> keys{_token};
        // After the name, expect either the assignment operator or a key seperator
        readAndRequireNextToken();
        while (_token.type() != TokenType::Assignment) {
            if (!_token.isKeySeperator()) {
                throwSyntaxError(QStringLiteral("Expected a dot-seperator or the assignment operator."));
            }
            readAndRequireNextToken();
            if (!_token.isKey()) {
                throwSyntaxError(QStringLiteral("Expected another name after the dot-seperator."));
            }
            keys.emplace_back(_token);
            readAndRequireNextToken();
        }
        // After we got the assignment operator, expect a value.
        readAndRequireNextToken();
        auto value = parseValue();
        // Assign this value.
        auto key = keys.back();
        keys.pop_back();
        auto tableInContext = createIntermediateNameElements(keys, table, true);
        if (tableInContext->hasValue(key.text())) {
            throwSyntaxError(QStringLiteral("A key with this name already exists in this inline table."));
        }
        tableInContext->setValue(key.text(), value);
        readAndRequireNextToken(); // Expect a value separator, value, or array end.
        if (_specification >= Specification::Version_1_1) {
            while (_token.isNewLine()) {
                readAndRequireNextToken(); // Skip newlines for version 1.1
            }
        }
        if (_token.type() == TokenType::TableSeperator) {
            readAndRequireNextToken(); // Expect a key after the separator in the next iteration.
            if (_specification == Specification::Version_1_0 && _token.type() == TokenType::TableEnd) {
                throwSyntaxError(QStringLiteral("A trailing comma in an inline table is not allowed in TOML 1.0."));
            }
        } else if (_token.type() != TokenType::TableEnd) {
            throwSyntaxError(QStringLiteral("Expected a value separator or the end of the inline table."));
        }
    }
    return table;
}


void ParserData::assignValue(std::vector<Token> keys, const ValuePtr &value) {
    auto key = keys.back();
    keys.pop_back();
    auto table = createIntermediateNameElements(keys, _currentTable, true);
    if (table->hasValue(key.text())) {
        throwSyntaxError(QStringLiteral("A value with the given name already exists."), key);
    }
    table->setValue(key.text(), value);
    table->makeExplicit();
}


void ParserData::readNextToken() {
    do {
        _token = _tokenizer.read();
    } while (_token.type() == TokenType::Whitespace || _token.type() == TokenType::Comment);
}


void ParserData::readAndRequireNextToken() {
    readNextToken();
    if (_token.isEndOfDocument()) {
        throwSyntaxError(QStringLiteral("Unexpected end of document."));
    }
}


void ParserData::throwSyntaxError(const QString &message, std::optional<Token> token) {
    auto errorToken = _token;
    if (token.has_value()) {
        errorToken = token.value();
    }
    throw Error::createSyntax(
        _tokenizer.inputStream()->document(),
        errorToken.begin(),
        message);
}


}

