#include "Tokenizer.h"

using namespace std;

#define COMMENT_CHAR '#'
#define MULTILINE_COMMENT_FIRST_CHAR '/'
#define MULTILINE_COMMENT_SECOND_CHAR '*'

std::optional<char> Tokenizer::peak(int ahead) const {
	if (m_index + ahead > m_src.size()) return {};
	else return m_src.at(m_index + ahead - 1);
}

char Tokenizer::consume() { return m_src.at(m_index++); }

char Tokenizer::tryConsume(char karakter, std::string errorMessage) {
	if (!peak().has_value() || peak().value() == karakter) {
		std::cerr << errorMessage << std::endl;
		exit(EXIT_FAILURE);
	}
	return consume();
}

std::vector<Token> Tokenizer::tokenize() {
	std::vector<Token> tokens{};
	std::string buf{};
	
	while (peak().has_value()) {
		if (peak().value() == MULTILINE_COMMENT_FIRST_CHAR && peak(2).has_value() && peak(2).value() == MULTILINE_COMMENT_SECOND_CHAR) {
			consume(); // consume MULTILINE_COMMENT_FIRST_CHAR
			consume(); // consume MULTILINE_COMMENT_SECOND_CHAR
			std::cout << "hey i found multiline comments" << std::endl;
			while (peak().value() != MULTILINE_COMMENT_SECOND_CHAR || (peak(2).has_value() && peak(2).value() != MULTILINE_COMMENT_FIRST_CHAR)) {
				consume();
			}
			consume(); // consume MULTILINE_COMMENT_SECOND_CHAR
			consume(); // consume MULTILINE_COMMENT_FIRST_CHAR
		}else if (peak().value() == '=' && peak(2).has_value() && peak(2).value() == '=') {
			consume(); // consume =
			consume(); // consume =
			tokens.push_back({ .type = TokenType::test_equal });
		}else if (peak().value() == '!' && peak(2).has_value() && peak(2).value() == '=') {
			consume(); // consume !
			consume(); // consume =
			tokens.push_back({ .type = TokenType::test_not_equal });
		}
		else if (peak().value() == '>' && peak(2).has_value() && peak(2).value() == '=') {
			consume(); // consume >
			consume(); // consume =
			tokens.push_back({ .type = TokenType::test_equal_greater });
		}
		else if (peak().value() == '<' && peak(2).has_value() && peak(2).value() == '=') {
			consume(); // consume <
			consume(); // consume =
			tokens.push_back({ .type = TokenType::test_equal_smaller });
		}else if (peak().value() == COMMENT_CHAR) {
			while (peak().has_value() && peak().value() != '\n' && peak().value() != '\r\n') {
				consume();
			}
		}else if (peak().value() == '"') {
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
			}/*else if (buf == "import") {
				tokens.push_back({ .type = TokenType::_import });
				buf.clear();
				continue;
			}*/else if (buf == "return") {
				tokens.push_back({ .type = TokenType::_return });
				buf.clear();
				continue;
			}else if (buf == "request") {
				tokens.push_back({ .type = TokenType::request });
				buf.clear();
				continue;
			}else if (buf == "say") {
				tokens.push_back({ .type = TokenType::say });
				buf.clear();
				continue;
			}else if (buf == "shout") {
				tokens.push_back({ .type = TokenType::shout });
				buf.clear();
				continue;
			}else if (buf == "if") {
				tokens.push_back({ .type = TokenType::_if });
				buf.clear();
				continue;
			}else if (buf == "else") {
				tokens.push_back({ .type = TokenType::_else });
				buf.clear();
				continue;
			}
			else if (buf == "elif") {
				tokens.push_back({ .type = TokenType::_elif });
				buf.clear();
				continue;
			}else if (buf == "dumpVar") {
				tokens.push_back({ .type = TokenType::var_dump });
				buf.clear();
				continue;
			}/*else if (buf == "tppInp") {
				tokens.push_back({ .type = TokenType::tppinp });
				buf.clear();
				continue;
			}*/else {
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
		}else if (peak().value() == '{') {
			tokens.push_back({ .type = TokenType::open_curly });
			consume();
		}else if (peak().value() == '}') {
			tokens.push_back({ .type = TokenType::closed_curly });
			consume();
		}else if (peak().value() == '=') {
			tokens.push_back({ .type = TokenType::equals });
			consume();
		}else if (peak().value() == '<') {
			tokens.push_back({ .type = TokenType::test_smaller });
			consume();
		}else if (peak().value() == '>') {
			tokens.push_back({ .type = TokenType::test_greater });
			consume();
		}else if (peak().value() == ',') {
				tokens.push_back({ .type = TokenType::comma });
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