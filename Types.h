#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

//administrative define's
#define NewLine "\n"
#define SEMI ';'
#define OPEN_PAREN '('
#define CLOSED_PAREN ')'
#define OPEN_CURLY '{'
#define CLOSED_CURLY '}'
#define EQUAL '='
#define NOT '!'
#define SMALLER '<'
#define LARGER '>'
#define COMMA ','
#define QUOTE '"'
#define ESCAPE_CHAR '\\'
#define ADDITION '+'
#define MULTIPLICATION '*'
#define SUBTRACTION '-'
#define DIVISION '/'


#define COMMENT_CHAR '#'
#define MULTILINE_COMMENT_FIRST_CHAR '/'
#define MULTILINE_COMMENT_SECOND_CHAR '*'

//define's defining the keywords
#define EXIT "exit"
#define SAY "say"
#define RETURN "return"
#define REQUEST "request"
#define SHOUT "shout"
#define IF "if"
#define ELIF "elif"
#define ELSE "else"
#define DUMPVAR "dumpVar"
#define TPPINP "tppInp"
#define TPPCOUNT "tppCount"

#define IMPORT "import"


enum class TokenType {
    _exit,
    int_lit,
    semi,
    open_Paren,
    closed_Paren,
    say,
    shout,
    string_lit,
    open_Quote,
    closed_Quote,
    _return,
    identifier,
    equals,
    open_curly,
    closed_curly,
    _if,
    _else,
    _elif,
    _import,
    request,
    test_equal,
    test_not_equal,
    test_equal_greater,
    test_equal_smaller,
    test_greater,
    test_smaller,
    comma,
    var_dump,
    tppinp,
    tppcount,
    addition,
    multiplication,
    subtraction,
    division
};

struct Token {
    TokenType type;
    std::optional<std::string> value{};
};

// Function to convert TokenType to string for printing
std::string TokenTypeToString(TokenType type);

// Define the output stream operator for Token
std::ostream& operator<<(std::ostream& os, const Token& token);