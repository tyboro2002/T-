#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include "Types.h"
#include "Parser.h"

class Generator {
	program m_prog;
	std::vector<std::string> m_defined_variabels;
public:
	Generator(program root) :m_prog(std::move(root)) {}
	[[nodiscard]] std::string generate();
private:
	std::string generateCodeLines(std::vector<std::variant<NodeExit, NodePrint, NodeReturn, NodeIdentifier, NodeScope>> codeLines);
};