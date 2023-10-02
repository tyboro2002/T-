#pragma once
#include <vector>
#include <variant>
#include "Types.h"

struct NodeExpr {
	Token int_lit_Identif;
};

struct NodeReturn {
	std::variant<NodeExpr, Token> retVal;
};

struct NodePrint {
	Token string_lit;
};

struct NodeExit {
	NodeExpr expr;
};

struct NodeIdentifier {
	std::string name;
	NodeExpr expr;
};

struct program {
	std::vector<std::variant<NodeExit, NodePrint, NodeReturn, NodeIdentifier>> codeLines;
};

class Parser {
	const std::vector<Token> m_tokens;
	int m_index;

public:
	Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)), m_index(0) {}
	std::optional<NodeExpr> parse_expr();
	std::optional<program> parse();
private:
	[[nodiscard]] std::optional<Token> peak(int ahead = 1) const;
	void parseOpenParen();
	void parseCloseParen();
	void parseOpenQuote();
	void parseCloseQuote();
	void parseEquals();
	void parseSemi();
	Token consume();
};