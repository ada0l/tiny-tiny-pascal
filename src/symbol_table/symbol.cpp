#include "symbol.h"

std::string Symbol::get_name() { return name; }
std::string Symbol::get_type_of_object_str() { return "---"; }
std::string Symbol::get_ret_type_str() { return "---"; }
void Symbol::draw_additional([[maybe_unused]] std::ostream& os,
                             [[maybe_unused]] int depth) {}
