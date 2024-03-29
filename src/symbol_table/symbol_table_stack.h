#ifndef SYMBOL_TABLE_STACK_H
#define SYMBOL_TABLE_STACK_H

#include "symbol_table.h"

class SymbolTableStack {
   public:
    SymbolTableStack() {}
    void push(std::string name, std::shared_ptr<Symbol> value);
    void push(const std::shared_ptr<Symbol> &&value);
    void push(std::shared_ptr<SymbolTable> table);
    void push(std::shared_ptr<parser::NodeId> id,
              std::shared_ptr<Symbol> symbol);
    void pop();
    std::shared_ptr<Symbol> get(std::string name);
    std::shared_ptr<Symbol> get_in_scope(std::string name);
    void alloc();
    void alloc_with_builtin();
    int size();
    void draw(std::ostream &os);

   private:
    std::vector<std::shared_ptr<SymbolTable>> data;
};

#endif
