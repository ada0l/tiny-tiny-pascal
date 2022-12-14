#include "token.h"

#include <algorithm>
#include <iomanip>
#include <magic_enum.hpp>
#include <sstream>

namespace scanner {
std::ostream &operator<<(std::ostream &os, const Operators &op) {
    os << magic_enum::enum_name(op);
    return os;
}

std::ostream &operator<<(std::ostream &os, const Separators &sep) {
    os << magic_enum::enum_name(sep);
    return os;
}

std::ostream &operator<<(std::ostream &os, const Keywords &keyword) {
    os << magic_enum::enum_name(keyword);
    return os;
}

std::ostream &operator<<(std::ostream &os, const TokenType &type) {
    os << magic_enum::enum_name(type);
    return os;
}

std::ostream &operator<<(std::ostream &os, const scanner::Token &token) {
    os << std::setw(10) << std::left << token.pos.first << std::setw(10)
       << std::left << token.pos.second << std::setw(20) << std::left
       << token.type << "\t";
    switch (token.type) {
        case TokenType::LITERAL_INTEGER:
            os << std::setw(30) << std::left << token.get_value<Integer>();
            break;
        case TokenType::LITERAL_DOUBLE:
            os << std::setw(30) << std::left << token.get_value<Double>();
            break;
        case TokenType::ID:
        case TokenType::LITERAL_STRING:
            os << std::setw(30) << std::left << token.get_value<String>();
            break;
        case TokenType::KEYWORD:
            os << std::setw(30) << std::left << token.get_value<Keywords>();
            break;
        case TokenType::COMMENT:
            break;
        case TokenType::OPER:
            os << std::setw(30) << std::left << token.get_value<Operators>();
            break;
        case TokenType::SEPERATOR:
            os << std::setw(30) << std::left << token.get_value<Separators>();
            break;
        case TokenType::eof:
            os << std::setw(30) << std::left << "EOF";
        case TokenType::INVALID:
            break;
    }
    os << std::setw(30) << std::left << token.raw_value;
    return os;
}

bool operator==(const Token &token, const TokenType &type) {
    return token.type == type;
}

bool operator==(const Token &token, const Operators &op) {
    if (token.type != TokenType::OPER) return false;
    return token.get_value<Operators>() == op;
}

bool operator==(const Token &token, const Separators &sep) {
    if (token.type != TokenType::SEPERATOR) return false;
    return token.get_value<Separators>() == sep;
}

bool operator==(const Token &token, const Keywords &keyword) {
    if (token.type != TokenType::KEYWORD) return false;
    return token.get_value<Keywords>() == keyword;
}

bool Token::is(const std::vector<TokenType> &types) {
    return std::find(types.begin(), types.end(), type) != types.end();
}

bool Token::is(const std::vector<Operators> &values) {
    if (type != TokenType::OPER) return false;
    return std::find(values.begin(), values.end(), get_value<Operators>()) !=
           values.end();
}

bool Token::is(const std::vector<Separators> &values) {
    if (type != TokenType::SEPERATOR) return false;
    return std::find(values.begin(), values.end(), get_value<Separators>()) !=
           values.end();
}

bool Token::is(const std::vector<Keywords> &values) {
    if (type != TokenType::KEYWORD) return false;
    return std::find(values.begin(), values.end(), get_value<Keywords>()) !=
           values.end();
}

std::string Token::to_str() const {
    std::ostringstream ss;
    ss << (*this);
    return ss.str();
}

TokenType Token::get_type() { return type; }

std::string Token::get_raw_value() { return raw_value; }

std::string Token::get_string_value() {
    if (*this == TokenType::KEYWORD) {
        auto buffer_in_lower = get_raw_value();
        std::transform(buffer_in_lower.begin(), buffer_in_lower.end(),
                       buffer_in_lower.begin(), ::tolower);
        return buffer_in_lower;
    }
    if (*this == TokenType::ID) {
        return get_value<std::string>();
    }
    return raw_value;
}

template <typename T>
T Token::get_value() const {
    return std::get<T>(value);
}

Position Token::get_pos() { return pos; }

void Token::stringify_value(TokenType type) {
    std::string buffer_in_lower = raw_value;
    std::transform(buffer_in_lower.begin(), buffer_in_lower.end(),
                   buffer_in_lower.begin(), ::tolower);
    value = buffer_in_lower;
    this->type = type;
}

}  // namespace scanner
