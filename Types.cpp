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
    case TokenType::open_Quote:
        return "open_Quote";
    case TokenType::closed_Quote:
        return "closed_Quote";
    case TokenType::string_lit:
        return "string_lit";
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