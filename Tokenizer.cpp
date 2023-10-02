#include "Tokenizer.h"

using namespace std;

std::optional<char> Tokenizer::peak(int ahead) const {
	if (m_index + ahead > m_src.size()) return {};
	else return m_src.at(m_index); //TODO test why no +ahead here
}

char Tokenizer::consume() { return m_src.at(m_index++); }

std::vector<Token> Tokenizer::tokenize() {
	std::vector<Token> tokens{};
	std::string buf{};
	
	while (peak().has_value()) {
		if (peak().value() == '"') {
			consume(); // consume the opening "
			tokens.push_back({ .type = TokenType::open_Quote });
			while (peak().has_value() && peak().value() != '"') {
				buf.push_back(consume());
			}
			consume(); // consume the closing "
			tokens.push_back({ .type = TokenType::string_lit, .value = buf });
			tokens.push_back({ .type = TokenType::closed_Quote });
			buf.clear();
		}else if (isalpha(peak().value())) {
			buf.push_back(consume());
			while (peak().has_value() && isalnum(peak().value())) {
				buf.push_back(consume());
			}

			if (buf == "exit") {
				tokens.push_back({ .type = TokenType::_exit });
				buf.clear();
				continue;
			}
			else if (buf == "return") {
				tokens.push_back({ .type = TokenType::_return });
				buf.clear();
				continue;
			}
			else if (buf == "say") {
				tokens.push_back({ .type = TokenType::say });
				buf.clear();
				continue;
			}
			else {
				tokens.push_back({ .type = TokenType::identifier, .value = buf });
				buf.clear();
				continue;
			}
		}
		else if (isdigit(peak().value())) {
			buf.push_back(consume());
			while (peak().has_value() && isdigit(peak().value())) {
				buf.push_back(consume());
			}
			tokens.push_back({ .type = TokenType::int_lit, .value = buf });
			buf.clear();
		}else if (peak().value() == ';') {
			tokens.push_back({ .type = TokenType::semi });
			consume();
		}else if (peak().value() == '(') {
			tokens.push_back({ .type = TokenType::open_Paren });
			consume();
		}else if (peak().value() == ')') {
			tokens.push_back({ .type = TokenType::closed_Paren });
			consume();
		}else if (peak().value() == '=') {
			tokens.push_back({ .type = TokenType::equals });
			consume();
		}else if (isspace(peak().value())) {
			consume();
			continue;
		}else {
			cerr << "you messed up with: " << buf << endl;
			exit(EXIT_FAILURE);
		}
	}
	return tokens;
}