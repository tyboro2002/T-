#pragma once
#include <fstream>
#include <vector>
#include <variant>
#include "Types.h"
#include "arena.h"

#define standAloneNode std::variant<NodeExit, NodeSay, NodeShout, NodeReturn, NodeIdentifier, NodeScope, NodeIf, NodeInput, NodeVarDump, NodeWhile>

int operatorToPrecedence(TokenType tokenType);

bool isBinOp(TokenType tokenType);

struct NodeTppInp {
	int number;
};

struct NodeVarDump {
	Token str_lit;
};

struct NodeExpr;

struct NodeBinExprAdd {
	NodeExpr* left;
	NodeExpr* right;
};

struct NodeBinExprMult {
	NodeExpr* left;
	NodeExpr* right;
};

struct NodeBinExprSub {
	NodeExpr* left;
	NodeExpr* right;
};

struct NodeBinExprDiv {
	NodeExpr* left;
	NodeExpr* right;
};

struct NodeBinExpr {
	std::variant<NodeBinExprAdd*, NodeBinExprMult*, NodeBinExprSub*, NodeBinExprDiv*> expr;
};

struct NodeTerm {
	std::variant<Token, NodeTppInp> term_part;
};
struct NodeExpr {
	std::variant<NodeTerm, NodeBinExpr*> exprPart;
};

struct NodeTest {
	NodeExpr* left_expr;
	Token test_expr;
	NodeExpr* right_expr;
};

struct NodeInput {
	Token identifier;
};

struct NodeReturn {
	std::variant<NodeExpr*, Token> retVal;
};

struct NodeSay {
	Token string_lit_identifier;
};

struct NodeShout {
	Token string_lit_identifier;
};

struct NodeExit {
	NodeExpr* expr;
};

struct NodeIdentifier {
	std::string name;
	std::optional<Token> compound;
	NodeExpr* expr;
};

struct NodeIf;
struct NodeWhile;

struct NodeScope {
	std::vector<standAloneNode> codeLines;
};

struct NodeElif {
	std::variant<NodeExpr*, NodeTest> expr;
	NodeScope scope;
};

struct NodeElse {
	NodeScope scope;
};

struct NodeWhile {
	std::variant<NodeExpr*, NodeTest> expr;
	NodeScope scope;
};

struct NodeIf {
	std::variant<NodeExpr*, NodeTest> expr;
	NodeScope scope;
	std::vector<NodeElif> elifs = {};
	std::optional<NodeElse> elsePart;
};

struct program {
	std::vector<std::string> imports;
	std::vector<standAloneNode> codeLines;
};

class Parser {
	const std::vector<Token> m_tokens; // the token vector
	int m_index; // index in the token vector
	ArenaAllocator m_allocator; // allocator for memory allocation of expresions
public:
	Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)), m_index(0), m_allocator(1024*1024*4) /* 4mb allocator*/ {}
	/* convert the vector of tokens to a program node */
	std::optional<program> parse();
private:
	/* peak <ahead> tokens ahead in the tokens vector (dont consume) */
	[[nodiscard]] std::optional<Token> peak(int ahead = 1) const;
	/* consume 1 token in the token vector and return it */
	Token consume();
	/* try consuming <tokenType> is found return <Token> else exit the program with <errorMessage> */
	Token tryConsume(TokenType tokenType, std::string errorMessage);
	bool isToken(TokenType type, int ahead = 1);
	bool isNotToken(TokenType type, int ahead = 1);
	void sayError(char ch);

	NodeVarDump parseVarDump();
	NodeReturn parseReturn();
	NodeIdentifier parseIdentifier();
	NodeSay parseSay();
	NodeShout parseShout();
	NodeExit parseExit();
	NodeInput parseInput();
	Token parseStringLit();
	NodeTppInp parseTppInp();
	std::optional<NodeTest> parseTest(NodeExpr* exprNodeLeft);
	std::optional<NodeElse> parseOptionalElse();
	std::vector<NodeElif> parseElifs();
	std::optional<NodeExpr*> parse_expr(int min_prec = 0);
	NodeWhile parseWhile();
	std::optional<NodeTerm> parse_term();
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