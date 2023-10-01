#include "CodeGenerator.h"

using namespace std;

std::string generateCCode(const vector<Token>& tokens) {
	std::stringstream out;
	out << "#include <stdio.h>" << NewLine;
	out << "#include <stdlib.h>" << NewLine << NewLine;
	out << "int main() {" << NewLine;
	for (int i = 0; i < tokens.size(); i++) {
		const Token& token = tokens.at(i);
		if (token.type == TokenType::_exit) {
			if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::open_Paren) {
				if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::int_lit) {
					if (i + 3 < tokens.size() && tokens.at(i + 3).type == TokenType::closed_Paren) {
						if (i + 4 < tokens.size() && tokens.at(i + 4).type == TokenType::semi) {
							out << "	exit(" << tokens.at(i + 2).value.value() << ");" << NewLine;
						}
					}
				}
			}
		}

		if (token.type == TokenType::_return) {
			if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::open_Paren) {
				if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::int_lit) {
					if (i + 3 < tokens.size() && tokens.at(i + 3).type == TokenType::closed_Paren) {
						if (i + 4 < tokens.size() && tokens.at(i + 4).type == TokenType::semi) {
							out << "	return(" << tokens.at(i + 2).value.value() << ");" << NewLine;
						}
					}
				}
			}
		}

		if (token.type == TokenType::_return) {
			if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::open_Paren) {
				if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::string_lit) {
					if (i + 3 < tokens.size() && tokens.at(i + 3).type == TokenType::closed_Paren) {
						if (i + 4 < tokens.size() && tokens.at(i + 4).type == TokenType::semi) {
							out << "	return(\"" << tokens.at(i + 2).value.value() << "\");" << NewLine;
						}
					}
				}
			}
		}

		if (token.type == TokenType::say) {
			if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::open_Paren) {
				if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::string_lit) {
					if (i + 3 < tokens.size() && tokens.at(i + 3).type == TokenType::closed_Paren) {
						if (i + 4 < tokens.size() && tokens.at(i + 4).type == TokenType::semi) {
							out << "	printf(\"" << tokens.at(i + 2).value.value() << "\");" << NewLine;
						}
					}
				}
			}
		}
	}
	out << "    return 0;" << NewLine;
	out << "}";
	return out.str();
}