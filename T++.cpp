// T++.cpp : Defines the entry point for the application.
// to start run
// cd out/build/x64-debug
// PS C:\Users\Gebruiker\source\repos\T++\out\build\x64-debug> ./T++ ../../../test.tpp
// to run the exe
// PS C:\Users\Gebruiker\source\repos\T++\out\build\x64-debug> ./../../../comp/output_program.exe

#include "T++.h"

using namespace std;

void printProgram(const program& prog) {
	std::cout << "Program:" << std::endl;
	for (const auto& codeLine : prog.codeLines) {
		if (std::holds_alternative<NodeExit>(codeLine)) {
			const NodeExit& exitNode = std::get<NodeExit>(codeLine);
			std::cout << "NodeExit: expr = " << exitNode.expr.int_lit_Identif << std::endl;
		}
		else if (std::holds_alternative<NodePrint>(codeLine)) {
			const NodePrint& printNode = std::get<NodePrint>(codeLine);
			std::cout << "NodePrint: string_lit = " << printNode.string_lit << std::endl;
		}
		else if (std::holds_alternative<NodeReturn>(codeLine)) {
			const NodeReturn& returnNode = std::get<NodeReturn>(codeLine);
			if (std::holds_alternative<NodeExpr>(returnNode.retVal)) {
				const NodeExpr& returnExprNode = std::get<NodeExpr>(returnNode.retVal);
				std::cout << "NodeReturn: expr = " << returnExprNode.int_lit_Identif << std::endl;
			}
			else if (std::holds_alternative<Token>(returnNode.retVal)) {
				const Token& returnToken = std::get<Token>(returnNode.retVal);
				if (returnToken.type == TokenType::string_lit) {
					std::cout << "NodeReturn: str = \"" << returnToken.value.value() << "\"" << std::endl;
				}
			}
		}else if (std::holds_alternative<NodeIdentifier>(codeLine)) {
			const NodeIdentifier& identifierNode = std::get<NodeIdentifier>(codeLine);
			std::cout << "NodeIdentifier: name = " << identifierNode.name << ", value = " << identifierNode.expr.int_lit_Identif.value.value() << std::endl;
		}
		else {
			std::cout << "Node in print i dont know" << std::endl;
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cerr << "Incorrect usage. Correct usage is ..." << endl;
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

	// Compile the generated C code using a C++ compiler (e.g., g++)
	//int compileStatus = std::system("gcc -mconsole ../../../comp/generated_code.c -o ../../../comp/output_program.exe");
	//int compileStatus = std::system("cd ../../../comp & dir & echo \"file contents:\" & type generated_code.c & gcc -mconsole generated_code.c -o output_program.exe");
	int compileStatus = std::system("cd ../../../comp & echo \"file contents:\" & type generated_code.c & gcc -mconsole generated_code.c -o output_program.exe");

	std::cout << std::endl;
	if (compileStatus == 0) {
		std::cout << "Compilation successful. Executable: 'output_program'" << std::endl;
	}
	else {
		std::cerr << "Compilation failed." << std::endl;
		return 1;
	}
	return EXIT_SUCCESS;
}

