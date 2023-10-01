// T++.cpp : Defines the entry point for the application.
// to start run
// cd out/build/x64-debug
// PS C:\Users\Gebruiker\source\repos\T++\out\build\x64-debug> ./T++ ../../../test.tpp
// to run the exe
// PS C:\Users\Gebruiker\source\repos\T++\out\build\x64-debug> ./../../../comp/output_program.exe

#include "T++.h"

using namespace std;

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

	// Generate C code
	std::string cCode = generateCCode(tokens);

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

