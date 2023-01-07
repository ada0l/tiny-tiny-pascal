#include "node_expression.h"

namespace parser {
void NodeId::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value() << " ";
}

std::string NodeId::get_name() { return token.get_value<std::string>(); }
Token& NodeId::get_token() { return token; }

void NodeBoolean::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value();
}

void NodeBinOp::draw(std::ostream& os, int depth) {
    os << token.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    left->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    right->draw(os, depth + 1);
}

std::shared_ptr<SymbolType> NodeBinOp::solve_casting() {
    auto left_st = left->get_sym_type();
    auto right_st = right->get_sym_type();
    // if one of the operands is an integer and a real
    if ((left_st->equivalent_to(SYMBOL_DOUBLE) &&
         right_st->equivalent_to(SYMBOL_INTEGER)) ||
        (left_st->equivalent_to(SYMBOL_INTEGER) &&
         right_st->equivalent_to(SYMBOL_DOUBLE))) {
        left = left_st->equivalent_to(SYMBOL_INTEGER)
                   ? std::make_shared<NodeCast>(left, SYMBOL_DOUBLE)
                   : left;
        right = right_st->equivalent_to(SYMBOL_INTEGER)
                    ? std::make_shared<NodeCast>(right, SYMBOL_DOUBLE)
                    : right;
        return left->get_sym_type();
    } else if (equivalent(SYMBOL_INTEGER, left_st, right_st)) {
        return SYMBOL_INTEGER;
    } else if (equivalent(SYMBOL_DOUBLE, left_st, right_st)) {
        return SYMBOL_DOUBLE;
    } else {
        return nullptr;
    }
}

std::shared_ptr<NodeExpression> NodeBinOp::get_left() { return left; }
std::shared_ptr<NodeExpression> NodeBinOp::get_right() { return right; }

void NodeUnOp::draw(std::ostream& os, int depth) {
    os << token.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    operand->draw(os, depth + 1);
}

void NodeRelOp::draw(std::ostream& os, int depth) {
    os << token.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    left->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    right->draw(os, depth + 1);
}

void NodeNumber::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value();
}

void NodeCast::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << "cast\n";
    draw_path(os, depth + 1);
    exp->draw(os, depth + 1);
}

void NodeString::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value();
}

void NodeFuncCall::draw(std::ostream& os, int depth) {
    var_ref->draw(os, depth);
    draw_vector(os, depth + 1, params);
}

std::shared_ptr<NodeVarRef> NodeFuncCall::get_var_ref() { return var_ref; }

std::vector<std::shared_ptr<NodeExpression>> NodeFuncCall::get_params() {
    return params;
}

void NodeArrayAccess::draw(std::ostream& os, int depth) {
    var_ref->draw(os, depth);
    os << "\n";
    draw_path(os, depth + 2);
    index->draw(os, depth + 2);
}

void NodeRecordAccess::draw(std::ostream& os, int depth) {
    var_ref->draw(os, depth);
    os << "\n";
    draw_path(os, depth + 1);
    field->draw(os, depth + 1);
}

void NodeSetSimpleElement::draw(std::ostream& os, int depth) {
    exp->draw(os, depth);
}

void NodeSetRangeElement::draw(std::ostream& os, int depth) {
    exp1->draw(os, depth);
    os << "..";
    exp2->draw(os, depth);
}

void NodeSetConstructor::draw(std::ostream& os, int depth) {
    draw_vector(os, depth, elements);
}
std::shared_ptr<NodeExpression> NodeCast::get_expression() { return exp; }
};  // namespace parser
