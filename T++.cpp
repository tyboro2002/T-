// T++.cpp : Defines the entry point for the application.
// to start run
// cd out/build/x64-debug
// PS C:\Users\Gebruiker\source\repos\T++\out\build\x64-debug> ./T++ ../../../test.tpp

#include "T++.h"

using namespace std;

std::vector<Token> tokenize(const std::string& str) {
	std::vector<Token> tokens{};
	std::string buf{};
	for (int i = 0; i < str.size(); i++) {
		char c = str[i];
		if (isalpha(c)) {
			buf.push_back(c);
			i++;
			while (isalnum(str[i])) {
				buf.push_back(str[i]);
				i++;
			}
			i--;

			if (buf == "return") {
				tokens.push_back({ .type = TokenType::_return });
				buf.clear();
				continue;
			}
			else {
				cerr << "you messed up with: " << buf << endl;
				exit(EXIT_FAILURE);
			}
		}
		else if (isdigit(c)) {
			buf.push_back(c);
			i++;
			while (isdigit(str[i])) {
				buf.push_back(str[i]);
				i++;
			}
			i--;
			tokens.push_back({ .type = TokenType::int_lit, .value = buf });
			buf.clear();
		}
		else if (c == ';') {
			tokens.push_back({ .type = TokenType::semi });
		}
		else if (isspace(c)) {
			continue;
		}
		else {
			cerr << "you messed up with: " << buf << endl;
			exit(EXIT_FAILURE);
		}

	}
	return tokens;
}

std::string generateCCode(const vector<Token>& tokens) {
	std::stringstream out;
	out << "#include <stdio.h>\n";
	out << "#include <stdlib.h>\n\n";
	out << "int main() {\n";
	out << "	printf(\"hello world!\");\n";
	for (int i = 0; i < tokens.size(); i++) {
		const Token& token = tokens.at(i);
		if (token.type == TokenType::_return) {
			if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit) {
				if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
					out << "	exit(" << tokens.at(i + 1).value.value() << ");\n";
				}
			}
		}
	}
	out << "    return 0;\n";
	out << "}";
	return out.str();
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
	std::vector<Token> tokens = tokenize(contents);
	for (Token token : tokens) {
		cout << token << endl;
	}

	// Generate C code
	std::string cCode = generateCCode(tokens);

	// Write the generated C code to a file
	std::ofstream outputFile("../../../comp/generated_code.c");

	outputFile << cCode;

	// Compile the generated C code using a C++ compiler (e.g., g++)
	//int compileStatus = std::system("gcc -mconsole ../../../comp/generated_code.c -o ../../../comp/output_program.exe");
	int compileStatus = std::system("cd ../../../comp & gcc -mconsole generated_code.c -o output_program.exe");

	if (compileStatus == 0) {
		std::cout << "Compilation successful. Executable: 'output_program'" << std::endl;
	}
	else {
		std::cerr << "Compilation failed." << std::endl;
		return 1;
	}
	return EXIT_SUCCESS;
}

