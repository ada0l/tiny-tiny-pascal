#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <iostream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace scanner {
using Integer = int;
using Double = double;
using String = std::string;

enum class Operators {
    ADD,        // +
    ADDASSIGN,  // +=
    SUB,        // -
    SUBASSIGN,  // -=
    MUL,        // *
    MULASSIGN,  // *=
    QUO,        // /
    QUOASSIGN,  // /=
    EQL,        // =
    LES,        // <
    NEQ,        // <>
    LEQ,        // <=
    GTR,        // >
    GEQ,        // >=
    ASSIGN,     // :=
    DEREF       // ^
};

enum class Separators {
    LPAREN,     // (
    RPAREN,     // )
    LBRACK,     // [
    RBRACK,     // ]
    COMMA,      // ,
    PERIOD,     // .
    ELLIPSIS,   // ..
    SEMICOLON,  // ;
    COLON       // :
};

enum class Keywords {
    ABSOLUTE,
    ABSTRACT,
    ALIAS,
    AND,
    ARRAY,
    AS,
    ASM,
    ASSEMBLER,
    BEGIN,
    BREAK,
    CASE,
    CDECL,
    CLASS,
    CONST,
    CONSTREF,
    CONSTRUCTOR,
    CONTINUE,
    CPPDECL,
    DEFAULT,
    DESTRUCTOR,
    DISPOSE,
    DIV,
    DO,
    DOWNTO,
    ELSE,
    END,
    EXCEPT,
    EXIT,
    EXPORT,
    EXPORTS,
    EXTERNAL,
    FALSE,
    FILE,
    FINALIZATION,
    FINALLY,
    FOR,
    FORWARD,
    FUNCTION,
    GENERIC,
    GOTO,
    IF,
    IMPLEMENTATION,
    IN,
    INDEX,
    INHERITED,
    INITIALIZATION,
    INLINE,
    INTERFACE,
    IS,
    LABEL,
    LIBRARY,
    LOCAL,
    MOD,
    NAME,
    NEW,
    NIL,
    NOSTACKFRAME,
    NOT,
    OBJECT,
    OF,
    OLDFPCCALL,
    ON,
    OPERATOR,
    OR,
    OUT,
    OVERRIDE,
    PACKED,
    PASCAL,
    PRIVATE,
    PROCEDURE,
    PROGRAM,
    PROPERTY,
    PROTECTED,
    PUBLIC,
    PUBLISHED,
    RAISE,
    READ,
    RECORD,
    REGISTER,
    REINTRODUCE,
    REPEAT,
    SAFECALL,
    SELF,
    SET,
    SHL,
    SHR,
    SOFTFLOAT,
    SPECIALIZE,
    STDCALL,
    STRING,
    THEN,
    THREADVAR,
    TO,
    TRUE,
    TRY,
    TYPE,
    UNIT,
    UNTIL,
    USES,
    VAR,
    VIRTUAL,
    WHILE,
    WITH,
    WRITE,
    XOR,
};

std::ostream &operator<<(std::ostream &os, const Operators &op);
std::ostream &operator<<(std::ostream &os, const Separators &sep);
std::ostream &operator<<(std::ostream &os, const Keywords &keyword);

using TokenValue =
    std::variant<Integer, Double, String, Operators, Separators, Keywords>;

enum class TokenType {
    eof,
    INVALID,
    LITERAL_INTEGER,
    LITERAL_DOUBLE,
    LITERAL_STRING,
    ID,
    KEYWORD,
    COMMENT,
    OPER,
    SEPERATOR,
};
std::ostream &operator<<(std::ostream &os, const TokenType &type);

class Token {
    unsigned int line, column;
    TokenType type;
    TokenValue value;
    std::string raw_value;  // the string being scanned

   public:
    Token(unsigned int line, unsigned int column, TokenType type,
          TokenValue value, std::string raw_value)
        : line(line),
          column(column),
          type(type),
          value(std::move(value)),
          raw_value(std::move(raw_value)){};

    friend std::ostream &operator<<(std::ostream &os, const Token &token);

    friend bool operator==(const Token &token, const TokenType &type);
    friend bool operator==(const Token &token, const Operators &op);
    friend bool operator==(const Token &token, const Separators &sep);

    bool is(const std::vector<TokenType> &types);
    bool is(const std::vector<Operators> &values);
    bool is(const std::vector<Separators> &values);

    [[nodiscard]] std::string to_string() const;

    template <typename T>
    T get_value() const;
    std::string get_raw_value();

    TokenType get_type();
};
}  // namespace scanner

#endif  // LEXER_TOKEN_H
