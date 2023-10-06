// T++.cpp : Defines the entry point for the application.
// to start run
// cd out/build/x64-debug
// PS C:\Users\Gebruiker\source\repos\T++\out\build\x64-debug> ./T++ ../../../test.tpp
// to run the exe
// PS C:\Users\Gebruiker\source\repos\T++\out\build\x64-debug> ./../../../comp/output_program.exe

#include "T++.h"

using namespace std;

// Function to convert a NodeExpr to a string suitable for the << operator
std::string convertNodeExpr(const NodeExpr node) {
	std::stringstream ss;
	if (std::holds_alternative<Token>(node.exprPart)) {
		const Token& token = std::get<Token>(node.exprPart);
		ss << token.value.value();
	}
	return ss.str();
}

// Function to convert a NodeExpr to a string suitable for the << operator
std::string convertNodeExprOrNodeTest(std::variant<NodeExpr, NodeTest> node) {
	std::stringstream ss;
	if (std::holds_alternative<NodeExpr>(node)) {
		const NodeExpr& expr_node = std::get<NodeExpr>(node);
		ss << convertNodeExpr(expr_node);
	}
	else if (std::holds_alternative<NodeTest>(node)) {
		const NodeTest& test_node = std::get<NodeTest>(node);
		ss << convertNodeExpr(test_node.left_expr);

		if (test_node.test_expr.type == TokenType::test_equal) {
			ss << "==";
		}
		else if (test_node.test_expr.type == TokenType::test_not_equal) {
			ss << "!=";
		}
		else if (test_node.test_expr.type == TokenType::test_equal_greater) {
			ss << ">=";
		}
		else if (test_node.test_expr.type == TokenType::test_equal_smaller) {
			ss << "<=";
		}
		else if (test_node.test_expr.type == TokenType::test_greater) {
			ss << ">";
		}
		else if (test_node.test_expr.type == TokenType::test_smaller) {
			ss << "<";
		}
		ss << convertNodeExpr(test_node.right_expr);
	}
	else {
		std::cerr << "invalid type in expr or test!" << std::endl;
		exit(EXIT_FAILURE);
	}
	return ss.str();
}

void printProgram(const program& prog) {
	std::cout << "Program:" << std::endl;
	for (const auto& codeLine : prog.codeLines) {
		if (std::holds_alternative<NodeExit>(codeLine)) {
			const NodeExit& exitNode = std::get<NodeExit>(codeLine);
			std::cout << "NodeExit: expr = " << convertNodeExpr(exitNode.expr) << std::endl;
		}else if (std::holds_alternative<NodeSay>(codeLine)) {
			const NodeSay& printNode = std::get<NodeSay>(codeLine);
			std::cout << "NodeSay: string_lit_identifier = " << printNode.string_lit_identifier << std::endl;
		}else if (std::holds_alternative<NodeShout>(codeLine)) {
			const NodeShout& printNode = std::get<NodeShout>(codeLine);
			std::cout << "NodeSchout: string_lit_identifier = " << printNode.string_lit_identifier << std::endl;
		}else if (std::holds_alternative<NodeReturn>(codeLine)) {
			const NodeReturn& returnNode = std::get<NodeReturn>(codeLine);
			if (std::holds_alternative<NodeExpr>(returnNode.retVal)) {
				const NodeExpr& returnExprNode = std::get<NodeExpr>(returnNode.retVal);
				std::cout << "NodeReturn: expr = " << convertNodeExpr(returnExprNode) << std::endl;
			}
			else if (std::holds_alternative<Token>(returnNode.retVal)) {
				const Token& returnToken = std::get<Token>(returnNode.retVal);
				if (returnToken.type == TokenType::string_lit) {
					std::cout << "NodeReturn: str = \"" << returnToken.value.value() << "\"" << std::endl;
				}
			}
		}else if (std::holds_alternative<NodeIdentifier>(codeLine)) {
			const NodeIdentifier& identifierNode = std::get<NodeIdentifier>(codeLine);
			std::cout << "NodeIdentifier: name = " << identifierNode.name << ", value = " << convertNodeExpr(identifierNode.expr) << std::endl;
		}else if (std::holds_alternative<NodeIf>(codeLine)) {
			const NodeIf& ifNode = std::get<NodeIf>(codeLine);
			std::cout << "ifNode: expr: " << convertNodeExprOrNodeTest(ifNode.expr) << std::endl;
			std::cout << "scopeNode open: ";
			printProgram(program{ .codeLines = ifNode.scope.codeLines});
			std::cout << "scopeNode close" << std::endl;
		}else if (std::holds_alternative<NodeScope>(codeLine)) {
			const NodeScope& scopeNode = std::get<NodeScope>(codeLine);
			std::cout << "scopeNode open: ";
			printProgram(program{.codeLines = scopeNode.codeLines});
			std::cout << "scopeNode close" << std::endl;
		}else if (std::holds_alternative<NodeInput>(codeLine)) {
			const NodeInput& inputNode = std::get<NodeInput>(codeLine);
			std::cout << "NodeInput: identifier = " << inputNode.identifier.value.value() << std::endl;
		}else {
			std::cout << "Node in print i dont know" << std::endl;
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc != 3 && argc != 2) {
		cerr << "Incorrect usage. Correct usage is one of following ..." << endl;
		cerr << "T++ <input.tpp> <output.exe>" << endl;
		cerr << "T++ <input.tpp>" << endl;
		return EXIT_FAILURE;
	}
	cout << argv[1] << endl;
	std::fstream input(argv[1], std::ios::in);
	std::stringstream contents_stream;
	contents_stream << input.rdbuf();
	input.close();
	std::string contents = contents_stream.str();
	cout << contents << endl;
	Tokenizer tokenizer(std::move(contents));
	std::vector<Token> tokens = tokenizer.tokenize();
	for (Token token : tokens) {
		cout << token << endl;
	}

	Parser parser(tokens);
	std::optional<program> prog = parser.parse();

	if (!prog.has_value()) {
		std::cerr << "parsing of the code failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "the program is" << std:: endl;
	printProgram(prog.value());

	// Generate C code
	Generator gen(prog.value());
	std::string cCode = gen.generate();

	// Write the generated C code to a file
	std::ofstream outputFile("../../../comp/generated_code.c");

	outputFile << cCode;
	outputFile.close();

	// Compile the generated C code using a C compiler (e.g., gcc)
	int compileStatus = std::system("cd ../../../comp & echo . & echo \"file contents:\" & type generated_code.c & gcc -mconsole generated_code.c -o ../output/output_program.exe");

	/*
	std::fstream inputCopy("output_program.exe", std::ios::in);
	std::stringstream contents_streamCopy;
	contents_streamCopy << inputCopy.rdbuf();


	std::ofstream outputFileCopy(argv[2]);
	outputFileCopy << inputCopy.rdbuf();

	inputCopy.close();
	outputFileCopy.close();
	*/

	std::cout << std::endl;
	if (compileStatus == 0) {
		if (argc == 3) {
			std::cout << "Compilation successful. Executable: '" << argv[2] << "'" << std::endl;
		}
		else {
			std::cout << "Compilation successful. Executable: 'output_program'" << std::endl;
		}
	}
	else {
		std::cerr << "Compilation failed." << std::endl;
		return 1;
	}
	return EXIT_SUCCESS;
}

