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

std::string tokens_to_asm(const vector<Token>& tokens) {
	std::stringstream out;
	//out << "section .text\n";
	out << "global main\n";
	out << "extern ExitProcess\n";
	out << "\n";
	out << "main:\n";
	for (int i = 0; i < tokens.size(); i++) {
		const Token& token = tokens.at(i);
		if (token.type == TokenType::_return) {
			if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit) {
				if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
					out << "	push " << tokens.at(i + 1).value.value() << "\n";
					out << "	call ExitProcess\n";
					//out << "	syscall";
				}
			}
		}

	}
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
	cout << endl << tokens_to_asm(tokens) << endl;

	std::fstream outputfile("../../../out.asm", std::ios::out);
	outputfile << tokens_to_asm(tokens);

	//system("cp ../../../out.asm ~/out.asm");
	//system("cd ~");
	//system("nasm -f elf64 ../../../out.asm");
	//system("ld -o ../../../out.exe ../../../out.o");
	
	
	//system("nasm -f win64 ../../../out.asm -o ../../../out.o");
	//system("link ../../../out.o /OUT:../../../out.exe /ENTRY:main");


	// Assemble the assembly code using NASM
	const char* nasmCommand = "nasm -f win64 ../../../out.asm -o ../../../out.obj";
	//const char* nasmCommand = "ml64 /c /Zi /Fo../../../out.obj ../../../out.asm";
	if (system(nasmCommand) == -1) {
		std::cerr << "Error assembling with NASM" << std::endl;
		return 1;
	}

	// Link the object file using the Microsoft linker
	//const char* linkerCommand = "link ../../../out.o /OUT:../../../out.exe /ENTRY:main";
	const char* linkerCommand = "link /subsystem:console /entry:main /OUT:../../../out.exe ../../../out.obj \"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0\\um\\x64\\kernel32.lib\"";

	if (system(linkerCommand) == -1) {
		std::cerr << "Error linking with the Microsoft linker" << std::endl;
		return 1;
	}

	std::cout << "Assembly and linking completed successfully." << std::endl;
	return EXIT_SUCCESS;
}

