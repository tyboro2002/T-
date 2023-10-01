#pragma once
#include <vector>
#include <variant>
#include "Types.h"

struct NodeExpr {
	Token int_lit;
};

struct NodeReturn {
	NodeExpr expr;
};

struct NodePrint {
	Token string_lit;
};

struct NodeExit {
	NodeExpr expr;
};

struct program {
	std::vector<std::variant<NodeExit, NodePrint, NodeReturn>> codeLines;
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
	void parseSemi();
	Token consume();
};