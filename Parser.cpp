#include "Parser.h"

std::optional<Token> Parser::peak(int ahead) const {
	if (m_index + ahead > m_tokens.size()) return {};
	else return m_tokens.at(m_index); //TODO test why no +ahead here
}

Token Parser::consume() { return m_tokens.at(m_index++);}

std::optional<NodeExpr> Parser::parse_expr() {
	std::optional<NodeExpr> expr_node;
	if (peak().has_value() && peak().value().type == TokenType::int_lit) {
		return NodeExpr{ .int_lit_Identif = consume() };
	}else if (peak().has_value() && peak().value().type == TokenType::identifier) {
		return NodeExpr{ .int_lit_Identif = consume() };
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
	if (!peak().has_value() || peak().value().type != TokenType::open_Quote) {
		std::cerr << "please open your string quotes." << NewLine << peak().value() << std::endl;
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseCloseQuote() {
	if (!peak().has_value() || peak().value().type != TokenType::closed_Quote) {
		std::cerr << "please close your string quote" << std::endl;
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseOpenCurly() {
	if (!peak().has_value() || peak().value().type != TokenType::open_curly) {
		std::cerr << "expected {" << std::endl;
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseCloseCurly() {
	if (!peak().has_value() || peak().value().type != TokenType::closed_curly) {
		std::cerr << "expected }" << std::endl;
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseEquals() {
	if (!peak().has_value() || peak().value().type != TokenType::equals) {
		std::cerr << "expecting =" << std::endl;
		exit(EXIT_FAILURE);
	}
	consume(); //consume the equals
}

void Parser::parseSemi() {
	if (!peak().has_value() || peak().value().type != TokenType::semi) {
		std::cerr << "expecting ;" << std::endl;
		exit(EXIT_FAILURE);
	}
	consume(); //consume the semicoln
}

NodeReturn Parser::parseReturn(){
	NodeReturn return_node;
	consume(); //consume the return node
	parseOpenParen();
	if (auto node_expr = parse_expr()) {
		return_node = NodeReturn{ .retVal = node_expr.value() };
	}
	else {
		std::cerr << "invalid expresion!" << std::endl;
		exit(EXIT_FAILURE);
	}
	parseCloseParen();
	parseSemi();
	return return_node;
}

NodeIdentifier Parser::parseIdentifier() {
	NodeIdentifier identifier_node;
	Token nameToken = consume();
	parseEquals();
	if (auto node_expr = parse_expr()) {
		identifier_node = NodeIdentifier{ .name = nameToken.value.value(), .expr = node_expr.value() };
	}
	else {
		std::cerr << "invalid expresion!" << std::endl;
		exit(EXIT_FAILURE);
	}
	parseSemi();
	return identifier_node;
}

NodePrint Parser::parseSay(){
	NodePrint print_node;
	consume();
	parseOpenParen();
	parseOpenQuote();
	if (peak().has_value() && peak().value().type == TokenType::string_lit) {
		print_node = NodePrint{ .string_lit = consume() };
	}
	parseCloseQuote();
	parseCloseParen();
	parseSemi();
	return print_node;
}

NodeExit Parser::parseExit() {
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
	return exit_node;
}

/*
std::optional<NodeIf> Parser::parseOptionalElseIfElse() {
	NodeIf ifNode;
	if (!peak().has_value() || peak().value().type != TokenType::_else) {
		return {};
	}
	consume(); //consume the if token
	if (peak().has_value() && peak().value().type == TokenType::_if) {
		parseOpenParen();
		if (auto node_expr = parse_expr()) {
			parseCloseParen();

			ifNode = NodeIf{ .expr = node_expr.value(), .scope = parseProgram(), .elsePart = parseOptionalElseIfElse()};
		}
		else {
			std::cerr << "invalid expresion!" << std::endl;
			exit(EXIT_FAILURE);
		}
		return ifNode;
	}
	else {
		ifNode = NodeIf{ .expr = NodeExpr{.int_lit_Identif = Token{.type = TokenType::int_lit, .value = "1"}}, .scope = parseProgram(), .elsePart = parseOptionalElseIfElse()};
		return ifNode;
	}
}
*/

NodeIf Parser::parseIf() {
	NodeIf ifNode;
	consume(); //consume the if token
	parseOpenParen();
	if (auto node_expr = parse_expr()) {
		parseCloseParen();
		ifNode = NodeIf{ .expr = node_expr.value(), .scope = parseProgram()/*, .elsePart = parseOptionalElseIfElse()*/};
	}
	else {
		std::cerr << "invalid expresion!" << std::endl;
		exit(EXIT_FAILURE);
	}
	return ifNode;
}

std::vector<standAloneNode> Parser::parseProgram(){
	program prog;
	while (peak().has_value()) {
		//std::cout << "token now is: " << peak().value() << std::endl;
		if (peak().value().type == TokenType::closed_curly) {
			return prog.codeLines;
		}else if (peak().value().type == TokenType::open_curly) {
			consume();
			prog.codeLines.push_back(NodeScope{ .codeLines = parseProgram() });
			parseCloseCurly();
		
		}else if (peak().value().type == TokenType::_exit) {
			prog.codeLines.push_back(parseExit()); // Add the exit node to the program
		}else if (peak().value().type == TokenType::say) {
			prog.codeLines.push_back(parseSay()); // Add the print node to the program
		}else if (peak().value().type == TokenType::_return) {
			prog.codeLines.push_back(parseReturn()); // Add the return node to the program
		}else if (peak().value().type == TokenType::identifier) {
			prog.codeLines.push_back(parseIdentifier()); // Add the identifier node to the program
		}else if (peak().value().type == TokenType::_if) {
			prog.codeLines.push_back(parseIf()); // Add the identifier node to the program
		}else {
			std::cerr << "found a token i dont like here, namely: " << peak().value() << std::endl; //TODO add parsing for extra nodes
			exit(EXIT_FAILURE);
		}
	}
	return prog.codeLines;
}

std::optional<program> Parser::parse() {
	return program{.codeLines = parseProgram()};
}
