#include "CodeGenerator.h"

using namespace std;

std::string Generator::generateCodeLines(std::vector<standAloneNode> inputCodeLines) {
	std::stringstream out;
	for (int i = 0; i < inputCodeLines.size(); i++) {
		const standAloneNode& variantNode = inputCodeLines.at(i);
		if (std::holds_alternative<NodeExit>(variantNode)) {
			const NodeExit& exitNode = std::get<NodeExit>(variantNode);
			out << "	exit(" << exitNode.expr.int_lit_Identif.value.value() << ");" << NewLine;
		}
		else if (std::holds_alternative<NodeReturn>(variantNode)) {
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
		}
		else if (std::holds_alternative<NodePrint>(variantNode)) {
			const NodePrint& printNode = std::get<NodePrint>(variantNode);
			out << "	printf(\"" << printNode.string_lit.value.value() << "\");" << NewLine;
		}
		else if (std::holds_alternative<NodeIdentifier>(variantNode)) {
			const NodeIdentifier& identifierNode = std::get<NodeIdentifier>(variantNode);
			// Check if the variable is already defined
			bool isAlreadyDefined = false;
			for (const std::string& definedVariable : m_defined_variabels) {
				if (definedVariable == identifierNode.name) {
					isAlreadyDefined = true;
					break;
				}
			}
			if (!isAlreadyDefined) {
				m_defined_variabels.push_back(identifierNode.name);
				out << "	int " << identifierNode.name << "=" << identifierNode.expr.int_lit_Identif.value.value() << ";" << NewLine;
			}
			else {
				out << "	" << identifierNode.name << "=" << identifierNode.expr.int_lit_Identif.value.value() << ";" << NewLine;
			}
		}
		else if (std::holds_alternative<NodeScope>(variantNode)) {
			const NodeScope& scopeNode = std::get<NodeScope>(variantNode);
			out << "{" << NewLine;
			if (scopeNode.codeLines.size() > 0) {
				out << generateCodeLines(scopeNode.codeLines);
			}
			out << "}" << NewLine;
		}else if (std::holds_alternative<NodeIf>(variantNode)) {
			const NodeIf& ifNode = std::get<NodeIf>(variantNode);
			out << "if (";
			out << ifNode.expr.int_lit_Identif.value.value();
			out << ")";
			if (ifNode.scope.codeLines.size() > 0) {
				out << generateCodeLines(ifNode.scope.codeLines);
			}
		}else {
			std::cerr << "you did some bad formating" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	return out.str();
}

std::string Generator::generate() {
	std::stringstream out;
	out << "#include <stdio.h>" << NewLine;
	out << "#include <stdlib.h>" << NewLine << NewLine;
	out << "int main() {" << NewLine;
	out << generateCodeLines(m_prog.codeLines);
	out << "    return 0;" << NewLine;
	out << "}";
	return out.str();
}