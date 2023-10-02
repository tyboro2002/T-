#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#define NewLine "\n"

enum class TokenType {
    _exit,
    int_lit,
    semi,
    open_Paren,
    closed_Paren,
    say,
    string_lit,
    open_Quote,
    closed_Quote,
    _return,
    identifier,
    equals
};

struct Token {
    TokenType type;
    std::optional<std::string> value{};
};

// Function to convert TokenType to string for printing
std::string TokenTypeToString(TokenType type);

// Define the output stream operator for Token
std::ostream& operator<<(std::ostream& os, const Token& token);