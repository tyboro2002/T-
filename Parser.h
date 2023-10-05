#pragma once
#include <fstream>
#include <vector>
#include <variant>
#include "Types.h"
#include "arena.h"

#define standAloneNode std::variant<NodeExit, NodePrint, NodeReturn, NodeIdentifier, NodeScope, NodeIf, NodeInput>

struct NodeExpr {
	std::variant<Token> exprPart;
};

struct NodeTest {
	NodeExpr left_expr;
	Token test_expr;
	NodeExpr right_expr;
};

struct NodeInput {
	Token identifier;
};

struct NodeReturn {
	std::variant<NodeExpr, Token> retVal;
};

struct NodePrint {
	Token string_lit_identifier;
};

struct NodeExit {
	NodeExpr expr;
};

struct NodeIdentifier {
	std::string name;
	NodeExpr expr;
};

struct NodeIf;

struct NodeScope {
	std::vector<standAloneNode> codeLines;
};

struct NodeElif {
	std::variant<NodeExpr, NodeTest> expr;
	NodeScope scope;
};

struct NodeElse {
	NodeScope scope;
};

struct NodeIf {
	std::variant<NodeExpr, NodeTest> expr;
	NodeScope scope;
	std::vector<NodeElif> elifs = {};
	std::optional<NodeElse> elsePart;
};

struct program {
	std::vector<std::string> imports;
	std::vector<standAloneNode> codeLines;
};

class Parser {
	const std::vector<Token> m_tokens;
	int m_index;
	ArenaAllocator m_allocator;
public:
	Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)), m_index(0), m_allocator(1024*1024*4) {}
	std::optional<NodeExpr> parse_expr();
	std::optional<program> parse();
private:
	[[nodiscard]] std::optional<Token> peak(int ahead = 1) const;
	Token consume();
	Token tryConsume(TokenType tokenType, std::string errorMessage);

	NodeReturn parseReturn();
	NodeIdentifier parseIdentifier();
	NodePrint parseSay();
	NodeExit parseExit();
	NodeInput parseInput();
	Token parseStringLit();
	std::optional<NodeTest> parseTest(NodeExpr exprNodeLeft);
	std::optional<NodeElse> parseOptionalElse();
	std::vector<NodeElif> parseElifs();
	NodeIf parseIf();
	std::vector<standAloneNode> parseProgram();
	void parseOpenParen();
	void parseCloseParen();
	void parseOpenQuote();
	void parseCloseQuote();
	void parseOpenCurly();
	void parseCloseCurly();
	void parseEquals();
	void parseSemi();
};