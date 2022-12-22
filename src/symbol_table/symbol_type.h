#ifndef SYMBOL_TYPE_H
#define SYMBOL_TYPE_H

#include "symbol.h"
#include "symbol_table.h"

class SymbolType : public Symbol {
   public:
    SymbolType(std::string name) : Symbol(name) {}
    ~SymbolType() override {}
    bool is_type() final { return true; }
    std::string get_type_of_object_str() override;
    std::string get_ret_type_str() override;
};

class SymbolBoolean : public SymbolType {
   public:
    SymbolBoolean() : SymbolType("boolean") {}
};

class SymbolInteger : public SymbolType {
   public:
    SymbolInteger() : SymbolType("integer") {}
};

class SymbolDouble : public SymbolType {
   public:
    SymbolDouble() : SymbolType("double") {}
};

class SymbolString : public SymbolType {
   public:
    SymbolString() : SymbolType("string") {}
};

class SymbolRecord : public SymbolType {
   public:
    SymbolRecord(std::shared_ptr<SymbolTable> fields)
        : SymbolType("record"), fields(std::move(fields)) {}
    std::string get_type_of_object_str() override;

   protected:
    std::shared_ptr<SymbolTable> fields;
};

class SymbolTypeAlias : public SymbolType {
   public:
    SymbolTypeAlias(std::string name, std::shared_ptr<SymbolType> original)
        : SymbolType(std::move(name)), original(std::move(original)) {}
    std::string get_type_of_object_str() override;

   protected:
    std::shared_ptr<SymbolType> original;
};
#endif
