#pragma once
#include <vector>
#include "Types.h"


struct NodeExpr {
	Token int_lit;
};

struct NodeExit {
	NodeExpr expr;
};

class Parser {
	const std::vector<Token> m_tokens;
	int m_index;

public:
	Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)), m_index(0) {}
	std::optional<NodeExpr> parse_expr();
	std::optional<NodeExit> parse();
private:
	[[nodiscard]] std::optional<Token> peak(int ahead = 1) const;
	Token consume();
};