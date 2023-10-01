#include "CodeGenerator.h"

using namespace std;

std::string Generator::generate() const {
	std::stringstream out;
	out << "#include <stdio.h>" << NewLine;
	out << "#include <stdlib.h>" << NewLine << NewLine;
	out << "int main() {" << NewLine;
	for (int i = 0; i < m_prog.codeLines.size(); i++) {
		const std::variant<NodeExit, NodePrint, NodeReturn>& variantNode = m_prog.codeLines.at(i);
		if (std::holds_alternative<NodeExit>(variantNode)) {
			const NodeExit& exitNode = std::get<NodeExit>(variantNode);
			out << "	exit(" << exitNode.expr.int_lit.value.value() << ");" << NewLine;
		}else if (std::holds_alternative<NodeReturn>(variantNode)) { //TODO add return string
			const NodeReturn& returnNode = std::get<NodeReturn>(variantNode);
			out << "	return(" << returnNode.expr.int_lit.value.value() << ");" << NewLine;
		}else if (std::holds_alternative<NodePrint>(variantNode)) {
			const NodePrint& printNode = std::get<NodePrint>(variantNode);
			out << "	printf(\"" << printNode.string_lit.value.value() << "\");" << NewLine;
		}
		else {
			std::cerr << "you did some bad formating" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	out << "    return 0;" << NewLine;
	out << "}";
	return out.str();
}