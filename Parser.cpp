#include "Parser.h"

std::optional<Token> Parser::peak(int ahead) const {
	if (m_index + ahead > m_tokens.size()) return {};
	else return m_tokens.at(m_index); //TODO test why no +ahead here
}

Token Parser::consume() { return m_tokens.at(m_index++);}

std::optional<NodeExpr> Parser::parse_expr() {
	std::optional<NodeExpr> expr_node;
	while (peak().has_value()) {
		if (peak().value().type == TokenType::_exit) { //TODO

		}
	}
	return expr_node;
}

std::optional<NodeExit> Parser::parse() {
	std::optional<NodeExit> exit_node;
	while (peak().has_value()) {
		if (peak().value().type == TokenType::_exit) { //TODO

		}
	}
	return exit_node;
}
