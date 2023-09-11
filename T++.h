// T++.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

// TODO: Reference additional headers your program requires here.

enum class TokenType{
	_return,
	int_lit,
	semi
};

struct Token{
	TokenType type;
	std::optional<std::string> value{};
};

// Function to convert TokenType to string for printing
std::string TokenTypeToString(TokenType type) {
    switch (type) {
    case TokenType::_return:
        return "_return";
    case TokenType::int_lit:
        return "int_lit";
    case TokenType::semi:
        return "semi";
        // Add more cases for other TokenType values as needed
    default:
        return "Unknown";
    }
}

// Define the output stream operator for Token
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