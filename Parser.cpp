#include "Parser.h"

std::optional<Token> Parser::peak(int ahead) const {
	if (m_index + ahead > m_tokens.size()) return {};
	else return m_tokens.at(m_index); //TODO test why no +ahead here
}

Token Parser::consume() { return m_tokens.at(m_index++);}

std::optional<NodeExpr> Parser::parse_expr() {
	std::optional<NodeExpr> expr_node;
	if (peak().has_value() && peak().value().type == TokenType::int_lit) {
		return NodeExpr{ .int_lit = consume() };
	}else {
		return {};
	}
	return expr_node;
}

void Parser::parseOpenParen() {
	if (!peak().has_value() || peak().value().type != TokenType::open_Paren) {
		std::cerr << "expecting (" << std::endl;
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseCloseParen() {
	if (!peak().has_value() || peak().value().type != TokenType::closed_Paren) {
		std::cerr << "expecting )" << std::endl;
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseOpenQuote() {
	if (!peak().has_value() || peak().value().type != TokenType::closed_Quote) {
		std::cerr << "please close your string quotes" << std::endl;
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseCloseQuote() {
	if (!peak().has_value() || peak().value().type != TokenType::open_Paren) {
		std::cerr << "expecting (" << std::endl;
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseSemi() {
	if (!peak().has_value() || peak().value().type != TokenType::semi) {
		std::cerr << "expecting ;" << std::endl;
		exit(EXIT_FAILURE);
	}
	consume(); //consume the semicoln
}

std::optional<program> Parser::parse() {
	program prog;
	//std::cout << std::endl << std::endl << std::endl;
	while (peak().has_value()) {
		//std::cout << "token now is: " << peak().value() << std::endl;
		if (peak().value().type == TokenType::_exit) {
			//std::cout << "exit detected" << std::endl;
			NodeExit exit_node;
			consume(); //consume the exit node
			if (auto node_expr = parse_expr()) {
				exit_node = NodeExit{ .expr = node_expr.value() };
			}
			else {
				std::cerr << "invalid expresion!" << std::endl;
				exit(EXIT_FAILURE);
			}
			parseSemi();

			// Add the exit node to the program
			prog.codeLines.push_back(exit_node);
		}else if(peak().value().type == TokenType::say){
			//std::cout << "say detected" << std::endl;
			NodePrint print_node;
			consume();
			parseOpenParen(); 
			parseOpenQuote();
			if (peak().has_value() && peak().value().type == TokenType::string_lit) {
				print_node = NodePrint{ .string_lit = consume()};
			}
			parseCloseQuote();
			parseCloseParen();
			parseSemi();
			// Add the print node to the program
			prog.codeLines.push_back(print_node);
		}else if (peak().value().type == TokenType::_return) {
			//std::cout << "exit detected" << std::endl;
			NodeReturn return_node;
			consume(); //consume the return node
			parseOpenParen();
			if (auto node_expr = parse_expr()) {
				return_node = NodeReturn{ .expr = node_expr.value() };
			}
			else {
				std::cerr << "invalid expresion!" << std::endl;
				exit(EXIT_FAILURE);
			}
			parseCloseParen();
			parseSemi();

			// Add the exit node to the program
			prog.codeLines.push_back(return_node);
		}else {
			std::cerr << "found a token i dont like here, namely: " << peak().value() << std::endl; //TODO add parsing for extra nodes
			exit(EXIT_FAILURE);
		}
	}
	return prog;
}
