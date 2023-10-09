#include "Types.h"

// Function to convert TokenType to string for printing
std::string TokenTypeToString(TokenType type) {
    switch (type) {
    case TokenType::_exit:
        return "_exit";
    case TokenType::_return:
        return "_return";
    case TokenType::int_lit:
        return "int_lit";
    case TokenType::semi:
        return "semi";
    case TokenType::open_Paren:
        return "open_Paren";
    case TokenType::closed_Paren:
        return "closed_Paren";
    case TokenType::say:
        return "say";
    case TokenType::shout:
        return "shout";
    case TokenType::open_Quote:
        return "open_Quote";
    case TokenType::closed_Quote:
        return "closed_Quote";
    case TokenType::string_lit:
        return "string_lit";
    case TokenType::identifier:
        return "identifier";
    case TokenType::equals:
        return "equals";
    case TokenType::open_curly:
        return "open_curly";
    case TokenType::closed_curly:
        return "closed_curly";
    case TokenType::_if:
        return "_if";
    case TokenType::_else:
        return "_else";
    case TokenType::_elif:
        return "_elif";
    case TokenType::_import:
        return "_import";
    case TokenType::request:
        return "request";
    case TokenType::test_equal:
        return "test_equal";
    case TokenType::test_not_equal:
        return "test_not_equal";
    case TokenType::test_equal_greater:
        return "test_equal_greater";
    case TokenType::test_equal_smaller:
        return "test_equal_smaller";
    case TokenType::test_greater:
        return "test_greater";
    case TokenType::test_smaller:
        return "test_smaller";
    case TokenType::comma:
        return "comma";
    case TokenType::var_dump:
        return "var_dump";
    case TokenType::tppinp:
        return "tppInp";
    case TokenType::tppcount:
        return "tppCount";
    case TokenType::addition:
        return "addition";
    case TokenType::multiplication:
        return "multiplication";
    case TokenType::subtraction:
        return "subtraction";
    case TokenType::division:
        return "division";
    // Add more cases for other TokenType values as needed
    default:
        return "Unknown";
    }
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "Token { type: " << TokenTypeToString(token.type) << ", value: ";

    // Check if the optional value has a value before outputting it
    if (token.value.has_value()) {
        os << token.value.value();
    }
    else {
        os << "nullopt";
    }

    os << " }";

    return os;
}