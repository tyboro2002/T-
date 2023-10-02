#include "CodeGenerator.h"

using namespace std;

std::string Generator::generate() const {
	std::stringstream out;
	out << "#include <stdio.h>" << NewLine;
	out << "#include <stdlib.h>" << NewLine << NewLine;
	out << "int main() {" << NewLine;
	for (int i = 0; i < m_prog.codeLines.size(); i++) {
		const std::variant<NodeExit, NodePrint, NodeReturn, NodeIdentifier>& variantNode = m_prog.codeLines.at(i);
		if (std::holds_alternative<NodeExit>(variantNode)) {
			const NodeExit& exitNode = std::get<NodeExit>(variantNode);
			out << "	exit(" << exitNode.expr.int_lit_Identif.value.value() << ");" << NewLine;
		}else if (std::holds_alternative<NodeReturn>(variantNode)) {
			const NodeReturn& returnNode = std::get<NodeReturn>(variantNode);
			if (std::holds_alternative<NodeExpr>(returnNode.retVal)) {
				const NodeExpr& returnExprNode = std::get<NodeExpr>(returnNode.retVal);
				out << "	return(" << returnExprNode.int_lit_Identif.value.value() << ");" << NewLine;
			}
			else if (std::holds_alternative<Token>(returnNode.retVal)) {
				const Token& returnTokenNode = std::get<Token>(returnNode.retVal);
				if (returnTokenNode.type == TokenType::string_lit) {
					out << "	return(\"" << returnTokenNode.value.value() << "\");" << NewLine;
				}
				else {
					std::cerr << "unsuported return type found: " << TokenTypeToString(returnTokenNode.type) << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		}else if (std::holds_alternative<NodePrint>(variantNode)) {
			const NodePrint& printNode = std::get<NodePrint>(variantNode);
			out << "	printf(\"" << printNode.string_lit.value.value() << "\");" << NewLine;
		}else if (std::holds_alternative<NodeIdentifier>(variantNode)) {
			const NodeIdentifier& identifierNode = std::get<NodeIdentifier>(variantNode);
			if (/*identifierNode.name in m_defined_variabels*/ true) {
				
				//m_defined_variabels.push_back(identifierNode.name); //TODO test if already defined and push in vector
				out << "	int " << identifierNode.name << "=" << identifierNode.expr.int_lit_Identif.value.value() << ";" << NewLine;
			}
			else {
				out << "	 " << identifierNode.name << "=" << identifierNode.expr.int_lit_Identif.value.value() << ";" << NewLine;
			}
		}else {
			std::cerr << "you did some bad formating" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	out << "    return 0;" << NewLine;
	out << "}";
	return out.str();
}