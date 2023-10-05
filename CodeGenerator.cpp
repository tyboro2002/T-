#include "CodeGenerator.h"

using namespace std;

// Function to convert a NodeExpr to a string suitable for the << operator
std::string Generator::convertNodeExpr(const NodeExpr node) {
	std::stringstream ss;
	if (std::holds_alternative<Token>(node.exprPart)) {
		const Token& token = std::get<Token>(node.exprPart);
		ss << token.value.value();
	}
	return ss.str();
}

// Function to convert a NodeExpr to a string suitable for the << operator
std::string Generator::convertNodeExprOrNodeTest(std::variant<NodeExpr, NodeTest> node) {
	std::stringstream ss;
	if (std::holds_alternative<NodeExpr>(node)) {
		const NodeExpr& expr_node = std::get<NodeExpr>(node);
		ss << convertNodeExpr(expr_node);
	}
	else if (std::holds_alternative<NodeTest>(node)) {
		const NodeTest& test_node = std::get<NodeTest>(node);
		ss << convertNodeExpr(test_node.left_expr);

		if (test_node.test_expr.type == TokenType::test_equal) {
			ss << " == ";
		}
		else if (test_node.test_expr.type == TokenType::test_not_equal) {
			ss << " != ";
		}
		else if (test_node.test_expr.type == TokenType::test_equal_greater) {
			ss << " >= ";
		}
		else if (test_node.test_expr.type == TokenType::test_equal_smaller) {
			ss << " <= ";
		}
		else if (test_node.test_expr.type == TokenType::test_greater) {
			ss << " > ";
		}
		else if (test_node.test_expr.type == TokenType::test_smaller) {
			ss << " < ";
		}
		ss << convertNodeExpr(test_node.right_expr);
	}
	else {
		std::cerr << "invalid type in expr or test!" << std::endl;
		exit(EXIT_FAILURE);
	}
	return ss.str();
}

std::string Generator::generateCodeLines(std::vector<standAloneNode> inputCodeLines) {
	std::stringstream out;
	for (int i = 0; i < inputCodeLines.size(); i++) {
		const standAloneNode& variantNode = inputCodeLines.at(i);
		if (std::holds_alternative<NodeExit>(variantNode)) {
			const NodeExit& exitNode = std::get<NodeExit>(variantNode);
			out << "	exit(" << convertNodeExpr(exitNode.expr) << ");" << NewLine;
		}else if (std::holds_alternative<NodeReturn>(variantNode)) {
			const NodeReturn& returnNode = std::get<NodeReturn>(variantNode);
			if (std::holds_alternative<NodeExpr>(returnNode.retVal)) {
				const NodeExpr& returnExprNode = std::get<NodeExpr>(returnNode.retVal);
				out << "	return(" << convertNodeExpr(returnExprNode) << ");" << NewLine;
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
			if (printNode.string_lit_identifier.type == TokenType::string_lit) {
				out << "	printf(\"" << printNode.string_lit_identifier.value.value() << "\\n\");" << NewLine;
			}else if (printNode.string_lit_identifier.type == TokenType::identifier) {
				out << "	printf(\"%d\\n\"," << printNode.string_lit_identifier.value.value() << "\);" << NewLine;
			}
		}else if (std::holds_alternative<NodeIdentifier>(variantNode)) {
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
				out << "	int " << identifierNode.name << " = " << convertNodeExpr(identifierNode.expr) << ";" << NewLine;
			}
			else {
				out << "	" << identifierNode.name << " = " << convertNodeExpr(identifierNode.expr) << ";" << NewLine;
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
			out << convertNodeExprOrNodeTest(ifNode.expr);
			out << "){" << NewLine;
			if (ifNode.scope.codeLines.size() > 0) {
				out << generateCodeLines(ifNode.scope.codeLines);
			}
			out << "}";
			if (ifNode.elifs.size() != 0) {
				// Iterate over all elif's
				for (const NodeElif& nodeElif : ifNode.elifs) {
					out << "else if (";
					out << convertNodeExprOrNodeTest(nodeElif.expr);
					out << "){" << NewLine;
					out << generateCodeLines(nodeElif.scope.codeLines);
					out << "}" << NewLine;
				}

			}
			if (ifNode.elsePart.has_value()) {
				out << "else{" << NewLine;
				out << generateCodeLines(ifNode.elsePart.value().scope.codeLines);
				out << "}" << NewLine;
			}
			else {
				out << NewLine;
			}
		}else if (std::holds_alternative<NodeInput>(variantNode)) {
			const NodeInput& inputNode = std::get<NodeInput>(variantNode);
			out << "	scanf(\"%d\",&" << inputNode.identifier.value.value() << ");" << NewLine;
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
	for (std::string& str : m_prog.imports) {
		out << "#include <" << str << ">" << NewLine;
	}
	out << "int main() {" << NewLine;
	out << generateCodeLines(m_prog.codeLines);
	out << "    return 0;" << NewLine;
	out << "}";
	return out.str();
}