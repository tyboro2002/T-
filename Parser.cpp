#include "Parser.h"

std::optional<Token> Parser::peak(int ahead) const {
	if (m_index + ahead > m_tokens.size()) return {};
	else return m_tokens.at(m_index + ahead - 1);
}

Token Parser::consume() { return m_tokens.at(m_index++);}

void Parser::sayError(char ch) {
	if (peak().has_value()) {
		std::cerr << "expected " << ch << " and found " << peak().value() << " after " << peak(0).value() << " at index " << m_index << std::endl;
	}
	else {
		std::cerr << "expected " << ch << " and found nothing" << std::endl;
	}
}

std::optional<NodeBinExpr*> Parser::parse_bin_expr() {
	if (auto left = parse_expr()) {
		auto binExpr = m_allocator.alloc<NodeBinExpr>();
		if (peak().has_value() && peak().value().type == TokenType::addition) {
			auto binExprAddition = m_allocator.alloc<NodeBinExprAdd>();
			binExprAddition->left = left.value();
			tryConsume(TokenType::addition, "expected +");
			if (auto right = parse_expr()) {
				binExprAddition->right = right.value();
				binExpr->expr = binExprAddition;
				return binExpr;
			}
			else {
				std::cerr << "no right expresion in binary expresion" << std::endl;
				exit(EXIT_FAILURE);
			}
		}else if (peak().has_value() && peak().value().type == TokenType::multiplication) {
			std::cerr << "unimplemented binary operator" << std::endl;
			exit(EXIT_FAILURE);
		}
		else {
			std::cerr << "unsoported binary operator" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	else {
		return {};
	}
}
std::optional<NodeTerm> Parser::parse_term() {
	NodeTerm nodeTerm;
	if (peak().has_value() && peak().value().type == TokenType::tppcount) {
		nodeTerm.term_part = tryConsume(TokenType::tppcount, "this tppCount disapeared");
	}
	else if (peak().has_value() && peak().value().type == TokenType::int_lit) {
		nodeTerm.term_part = tryConsume(TokenType::int_lit, "this int_lit disapeared");
	}
	else if (peak().has_value() && peak().value().type == TokenType::open_Quote) {
		tryConsume(TokenType::open_Quote, "expected open quote");
		nodeTerm.term_part = tryConsume(TokenType::string_lit, "this string_lit disapeared");
		tryConsume(TokenType::closed_Quote, "expected closing quote");
	}
	else if (peak().has_value() && peak().value().type == TokenType::identifier) {
		nodeTerm.term_part = tryConsume(TokenType::identifier, "this identifier disapeared");
	}
	else if (peak().has_value() && peak().value().type == TokenType::tppinp) {
		nodeTerm.term_part = parseTppInp();
	}
	else {
		return {};
	}
	return nodeTerm;
}

std::optional<NodeExpr*> Parser::parse_expr() {
	NodeExpr* nodeExpr = m_allocator.alloc<NodeExpr>();
	//std::cout << "token is: " << peak().value() << std::endl;
	if (auto term = parse_term()) {
		if (peak().has_value() && (peak().value().type == TokenType::addition || peak().value().type == TokenType::multiplication)) {
			auto binExpr = m_allocator.alloc<NodeBinExpr>();
			if (peak().has_value() && peak().value().type == TokenType::addition) {
				NodeBinExprAdd* binExprAddition = m_allocator.alloc<NodeBinExprAdd>();
				NodeExpr* left = m_allocator.alloc<NodeExpr>();
				left->exprPart = term.value();
				binExprAddition->left = left;
				tryConsume(TokenType::addition, "expected +");
				if (auto right = parse_expr()) {
					binExprAddition->right = right.value();
					binExpr->expr = binExprAddition;
					nodeExpr->exprPart = binExpr;
					return nodeExpr;
				}
				else {
					std::cerr << "no right expresion in binary expresion" << std::endl;
					exit(EXIT_FAILURE);
				}
			}/*else if (peak().has_value() && peak().value().type == TokenType::multiplication) {
				std::cerr << "unimplemented binary operator" << std::endl;
				exit(EXIT_FAILURE);
			}*/
			else {
				std::cerr << "unsoported binary operator" << std::endl;
				exit(EXIT_FAILURE);
			}
		}else {
			nodeExpr->exprPart = term.value();
			return nodeExpr;
		}
	}
	else {
		std::cerr << "unable to parse term" << std::endl;
		exit(EXIT_FAILURE);
	}
	return nodeExpr;
}

void Parser::parseOpenParen() {
	if (!peak().has_value() || peak().value().type != TokenType::open_Paren) {
		sayError(OPEN_PAREN);
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseCloseParen() {
	if (!peak().has_value() || peak().value().type != TokenType::closed_Paren) {
		sayError(CLOSED_PAREN);
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseOpenQuote() {
	if (!peak().has_value() || peak().value().type != TokenType::open_Quote) {
		sayError(QUOTE);
		std::cerr << "please open your string quotes." << NewLine << peak().value() << std::endl;
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseCloseQuote() {
	if (!peak().has_value() || peak().value().type != TokenType::closed_Quote) {
		sayError(QUOTE);
		std::cerr << "please close your string quote" << std::endl;
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseOpenCurly() {
	if (!peak().has_value() || peak().value().type != TokenType::open_curly) {
		sayError(OPEN_CURLY);
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseCloseCurly() {
	if (!peak().has_value() || peak().value().type != TokenType::closed_curly) {
		sayError(CLOSED_CURLY);
		exit(EXIT_FAILURE);
	}
	consume();
}

void Parser::parseEquals() {
	if (!peak().has_value() || peak().value().type != TokenType::equals) {
		sayError(EQUAL);
		exit(EXIT_FAILURE);
	}
	consume(); //consume the equals
}

void Parser::parseSemi() {
	if (!peak().has_value() || peak().value().type != TokenType::semi) {
		sayError(SEMI);
		exit(EXIT_FAILURE);
	}
	consume(); //consume the semicoln
}

Token Parser::tryConsume(TokenType tokenType, std::string errorMessage) {
	if (!peak().has_value() || peak().value().type != tokenType) {
		std::cerr << errorMessage << std::endl;
		exit(EXIT_FAILURE);
	}
	return consume();
}

NodeReturn Parser::parseReturn(){
	NodeReturn return_node;
	tryConsume(TokenType::_return, "expected return"); //consume the return node
	parseOpenParen();
	if (auto node_expr = parse_expr()) {
		return_node = NodeReturn{ .retVal = node_expr.value() };
	}
	else {
		std::cerr << "invalid expresion encouterd in return!" << std::endl;
		exit(EXIT_FAILURE);
	}
	parseCloseParen();
	parseSemi();
	return return_node;
}

NodeIdentifier Parser::parseIdentifier() {
	NodeIdentifier identifier_node;
	Token nameToken = tryConsume(TokenType::identifier, "expected identifier");
	parseEquals();
	if (auto node_expr = parse_expr()) {
		identifier_node = NodeIdentifier{ .name = nameToken.value.value(), .expr = node_expr.value() };
	}
	else {
		std::cerr << "invalid expresion encounterd in identifier!" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	parseSemi();
	return identifier_node;
}

NodeSay Parser::parseSay(){
	NodeSay print_node;
	tryConsume(TokenType::say, "expected say");
	parseOpenParen();
	if (peak().has_value() && peak().value().type == TokenType::identifier) {
		print_node = NodeSay{ .string_lit_identifier = tryConsume(TokenType::identifier, "this identifier disapeared") };
	}
	else {
		parseOpenQuote();
		if (peak().has_value() && peak().value().type == TokenType::string_lit) {
			print_node = NodeSay{ .string_lit_identifier = tryConsume(TokenType::string_lit, "this string_lit disapeared") };
		}
		parseCloseQuote();
	}
	parseCloseParen();
	parseSemi();
	return print_node;
}


NodeShout Parser::parseShout() {
	NodeShout print_node;
	tryConsume(TokenType::shout, "expected shout");
	parseOpenParen();
	if (peak().has_value() && peak().value().type == TokenType::identifier) {
		print_node = NodeShout{ .string_lit_identifier = tryConsume(TokenType::identifier, "this identifier disapeared") };
	}
	else {
		parseOpenQuote();
		if (peak().has_value() && peak().value().type == TokenType::string_lit) {
			print_node = NodeShout{ .string_lit_identifier = tryConsume(TokenType::string_lit, "this string_lit disapeared") };
		}
		parseCloseQuote();
	}
	parseCloseParen();
	parseSemi();
	return print_node;
}

NodeExit Parser::parseExit() {
	NodeExit exit_node;
	tryConsume(TokenType::_exit, "expected exit"); //consume the exit node
	parseOpenParen();
	if (auto node_expr = parse_expr()) {
		exit_node = NodeExit{ .expr = node_expr.value() };
	}
	else {
		std::cerr << "invalid expresion encounterd in exit!" << std::endl;
		exit(EXIT_FAILURE);
	}
	parseCloseParen();
	parseSemi();
	return exit_node;
}


std::optional<NodeElse> Parser::parseOptionalElse() {
	NodeElse elseNode;
	if (!peak().has_value() || peak().value().type != TokenType::_else) {
		return {};
	}
	tryConsume(TokenType::_else, "expected else"); // consume the else token
	parseOpenCurly(); // consume the open curly
	elseNode = NodeElse{ .scope = parseProgram() };
	parseCloseCurly(); // consume the closed curly
	return elseNode;
}

std::vector<NodeElif> Parser::parseElifs() {
	std::vector<NodeElif> elifs;
	NodeElif elifNode;// = m_allocator.alloc<NodeElif>();
	while (peak().has_value() && peak().value().type == TokenType::_elif) {
		tryConsume(TokenType::_elif, "expected elif"); // consume the elif
		parseOpenParen();
		if (auto node_expr = parse_expr()) {
			NodeExpr* exprNodeLeft =node_expr.value();
			if (auto node_test = parseTest(exprNodeLeft)) {
				elifNode.expr = node_test.value();
			}
			else {
				elifNode.expr = exprNodeLeft;
			}
			parseCloseParen();
			parseOpenCurly();
			elifNode.scope.codeLines = parseProgram();
			parseCloseCurly();
		}
		else {
			std::cerr << "invalid expresion encounterd in elif!" << std::endl;
			exit(EXIT_FAILURE);
		}
		elifs.push_back(elifNode);
	}
	return elifs;
}

NodeIf Parser::parseIf() {
	NodeIf ifNode;
	tryConsume(TokenType::_if, "expected if"); //consume the if token
	parseOpenParen();
	if (auto node_expr = parse_expr()) {
		NodeExpr* exprNodeLeft = node_expr.value();
		if (auto node_test = parseTest(exprNodeLeft)) {
			parseCloseParen();
			parseOpenCurly();
			ifNode = NodeIf{ .expr = node_test.value(), .scope = parseProgram()};
		}
		else {
			parseCloseParen();
			parseOpenCurly();
			ifNode = NodeIf{ .expr = exprNodeLeft, .scope = parseProgram() };
		}
		parseCloseCurly();
		if (peak().has_value() && peak().value().type == TokenType::_elif) {
			ifNode.elifs = parseElifs();
		}
		ifNode.elsePart = parseOptionalElse();
	}
	else{
		std::cerr << "invalid expresion encounterd in if!" << std::endl;
		exit(EXIT_FAILURE);
	}
	return ifNode;
}


NodeInput Parser::parseInput() {
	NodeInput nodeInp;
	tryConsume(TokenType::request, "expected request");
	parseOpenParen();
	nodeInp.identifier = tryConsume(TokenType::identifier, "expected an identifier in the request");
	parseCloseParen();
	parseSemi();
	return nodeInp;
}

NodeVarDump Parser::parseVarDump() {
	NodeVarDump nodeVarDump;
	tryConsume(TokenType::var_dump, "expected var_dump");
	parseOpenParen();
	parseOpenQuote();
	nodeVarDump.str_lit = tryConsume(TokenType::string_lit, "expected a string lit");
	parseCloseQuote();
	parseCloseParen();
	parseSemi();
	return nodeVarDump;
}


NodeTppInp Parser::parseTppInp() {
	NodeTppInp nodeTppInp;
	tryConsume(TokenType::tppinp, "expected tppInp");
	parseOpenParen();
	nodeTppInp.number = std::stoi(tryConsume(TokenType::int_lit, "expected a int lit").value.value());
	parseCloseParen();
	return nodeTppInp;
}



std::vector<standAloneNode> Parser::parseProgram(){
	program prog;
	while (peak().has_value()) {
		//std::cout << "token now is: " << peak().value() << std::endl;
		if (peak().value().type == TokenType::closed_curly) {
			return prog.codeLines;
		}else if (peak().value().type == TokenType::open_curly) {
			tryConsume(TokenType::open_curly, "this open curly disapeared");
			prog.codeLines.push_back(NodeScope{ .codeLines = parseProgram() });
			parseCloseCurly();
		
		}else if (peak().value().type == TokenType::request) {
			prog.codeLines.push_back(parseInput()); // Add the input node to the program
		}else if (peak().value().type == TokenType::_exit) {
			prog.codeLines.push_back(parseExit()); // Add the exit node to the program
		}else if (peak().value().type == TokenType::say) {
			prog.codeLines.push_back(parseSay()); // Add the say node to the program
		}else if (peak().value().type == TokenType::shout) {
			prog.codeLines.push_back(parseShout()); // Add the shout node to the program
		}else if (peak().value().type == TokenType::_return) {
			prog.codeLines.push_back(parseReturn()); // Add the return node to the program
		}else if (peak().value().type == TokenType::identifier) {
			prog.codeLines.push_back(parseIdentifier()); // Add the identifier node to the program
		}else if (peak().value().type == TokenType::_if) {
			prog.codeLines.push_back(parseIf()); // Add the if node to the program
		}else if (peak().value().type == TokenType::var_dump) {
			prog.codeLines.push_back(parseVarDump()); // Add the varDump node to the program
		}else {
			std::cerr << "found a token i dont like here, namely: " << peak().value() << std::endl; //TODO add parsing for extra nodes
			exit(EXIT_FAILURE);
		}
	}
	return prog.codeLines;
}

Token Parser::parseStringLit() {
	Token str;
	parseOpenQuote();
	if (peak().has_value() && peak().value().type == TokenType::string_lit) {
		str = tryConsume(TokenType::string_lit, "this string_lit disapeared");
	}
	else {
		std::cerr << "invalid string!" << std::endl;
		exit(EXIT_FAILURE);
	}
	parseCloseQuote();
	return str;
}

std::optional<NodeTest> Parser::parseTest(NodeExpr* exprNodeLeft) {
	NodeTest nodeTest;//NodeTest* nodeTest = m_allocator.alloc<NodeTest>();
	nodeTest.left_expr = exprNodeLeft;
	if (peak().has_value() && peak().value().type == TokenType::test_equal) {
		consume();
		nodeTest.test_expr = Token{ .type = TokenType::test_equal };
	}else if (peak().has_value() && peak().value().type == TokenType::test_not_equal) {
		consume();
		nodeTest.test_expr = Token{ .type = TokenType::test_not_equal };
	}else if (peak().has_value() && peak().value().type == TokenType::test_equal_greater) {
		consume();
		nodeTest.test_expr = Token{ .type = TokenType::test_equal_greater };
	}else if (peak().has_value() && peak().value().type == TokenType::test_equal_smaller) {
		consume();
		nodeTest.test_expr = Token{ .type = TokenType::test_equal_smaller };
	}else if (peak().has_value() && peak().value().type == TokenType::test_greater) {
		consume();
		nodeTest.test_expr = Token{ .type = TokenType::test_greater };
	}else if (peak().has_value() && peak().value().type == TokenType::test_smaller) {
		consume();
		nodeTest.test_expr = Token{ .type = TokenType::test_smaller };
	}else {
		return {};
	}
	if (auto node_expr = parse_expr()) {
		nodeTest.right_expr = node_expr.value();
	}
	else {
		std::cerr << "expresion missing after: " << nodeTest.test_expr << std::endl;
		exit(EXIT_FAILURE);
	}
	return nodeTest;
}

std::optional<program> Parser::parse() {
	program prog;
	while (peak().has_value() && peak().value().type == TokenType::_import) {
		tryConsume(TokenType::_import, "this import disapeared"); // consume the import
		prog.imports.push_back(parseStringLit().value.value());
		parseSemi(); //consuming the semicoln
	}
	prog.codeLines = parseProgram();
	return prog;
}
