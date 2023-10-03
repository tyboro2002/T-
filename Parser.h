#pragma once
#include <fstream>
#include <vector>
#include <variant>
#include "Types.h"

#define standAloneNode std::variant<NodeExit, NodePrint, NodeReturn, NodeIdentifier, NodeScope, NodeIf>

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

struct NodeIf;

struct NodeScope {
	std::vector<standAloneNode> codeLines;
};

struct NodeElif {
	NodeExpr expr;
	NodeScope scope;
};

struct NodeElse {
	NodeScope scope;
};

struct NodeIf {
	NodeExpr expr;
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

public:
	Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)), m_index(0) {}
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
	Token parseStringLit();
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