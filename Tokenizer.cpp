#include "Tokenizer.h"

using namespace std;

std::optional<char> Tokenizer::peak(int ahead) const {
	if (m_index + ahead > m_src.size()) return {};
	else return m_src.at(m_index + ahead - 1);
}

char Tokenizer::consume() { return m_src.at(m_index++); }
void Tokenizer::discard(int amount) { m_index+=amount; }

char Tokenizer::tryConsume(char karakter, std::string errorMessage) {
	if (!peak().has_value() || peak().value() == karakter) {
		std::cerr << errorMessage << std::endl;
		exit(EXIT_FAILURE);
	}
	return consume();
}

bool Tokenizer::isChar(char karakter, int ahead) {
	return peak(ahead).has_value() && peak(ahead).value() == karakter;
}

bool Tokenizer::isNotChar(char karakter, int ahead) {
	return !peak(ahead).has_value() || peak(ahead).value() != karakter;
}

std::vector<Token> Tokenizer::tokenize() {
	std::vector<Token> tokens{};
	std::string buf{};
	
	while (peak().has_value()) {
		if (isChar(MULTILINE_COMMENT_FIRST_CHAR) && isChar(MULTILINE_COMMENT_SECOND_CHAR,2)) {
			discard(2); // consume MULTILINE_COMMENT_FIRST_CHAR consume MULTILINE_COMMENT_SECOND_CHAR
			while (isNotChar(MULTILINE_COMMENT_SECOND_CHAR) || isNotChar(MULTILINE_COMMENT_FIRST_CHAR,2)) {
				if (!peak().has_value()) {
					std::cerr << "found unexpected EOF while parsing multiline comment" << std::endl;
					exit(EXIT_FAILURE);
				}
				discard();
			}
			discard(2); // consume MULTILINE_COMMENT_SECOND_CHAR consume MULTILINE_COMMENT_FIRST_CHAR
		}else if (isChar(EQUAL) && isChar(EQUAL,2)) {
			discard(2); // consume = consume =
			tokens.push_back({ .type = TokenType::test_equal });
		}else if (isChar(NOT) && isChar(EQUAL,2)) {
			discard(2); // consume ! consume =
			tokens.push_back({ .type = TokenType::test_not_equal });
		}else if (isChar(LARGER) && isChar(EQUAL,2)) {
			discard(2); // consume > consume =
			tokens.push_back({ .type = TokenType::test_equal_greater });
		}else if (isChar(SMALLER) && isChar(EQUAL,2)) {
			discard(2); // consume < consume =
			tokens.push_back({ .type = TokenType::test_equal_smaller });
		}else if (isChar(ADDITION) && isChar(EQUAL, 2)) {
			discard(2); // consume + consume =
			tokens.push_back({ .type = TokenType::compound_add });
		}else if (isChar(SUBTRACTION) && isChar(EQUAL, 2)) {
			discard(2); // consume - consume =
			tokens.push_back({ .type = TokenType::compound_sub });
		}else if (isChar(DIVISION) && isChar(EQUAL, 2)) {
			discard(2); // consume / consume =
			tokens.push_back({ .type = TokenType::compound_div });
		}else if (isChar(MULTIPLICATION) && isChar(EQUAL, 2)) {
			discard(2); // consume * consume =
			tokens.push_back({ .type = TokenType::compound_mult });
		}else if (isChar(MODULUS) && isChar(EQUAL, 2)) {
			discard(2); // consume % consume =
			tokens.push_back({ .type = TokenType::compound_modulus });
		}else if (isChar(BITWISE_AND) && isChar(EQUAL, 2)) {
			discard(2); // consume & consume =
			tokens.push_back({ .type = TokenType::compound_bitwise_and });
		}else if (isChar(BITWISE_OR) && isChar(EQUAL, 2)) {
			discard(2); // consume | consume =
			tokens.push_back({ .type = TokenType::compound_bitwise_or });
		}else if (isChar(BITWISE_XOR) && isChar(EQUAL, 2)) {
			discard(2); // consume ^ consume =
			tokens.push_back({ .type = TokenType::compound_bitwise_xor });
		}else if (isChar(SMALLER) && isChar(SMALLER, 2) && isChar(EQUAL,3)) {
			discard(3); // consume < consume < consume =
			tokens.push_back({ .type = TokenType::compound_left_shift });
		}else if (isChar(LARGER) && isChar(LARGER, 2) && isChar(EQUAL, 3)) {
			discard(3); // consume > consume > consume =
			tokens.push_back({ .type = TokenType::compound_right_shift });
		}else if (isChar(COMMENT_CHAR)) {
			while (peak().has_value() && peak().value() != '\n' && peak().value() != '\r\n') {
				discard();
			}
		}else if (isChar(QUOTE)) {
			discard(); // consume the opening "
			tokens.push_back({ .type = TokenType::open_Quote });
			while (peak().has_value() && peak().value() != QUOTE) {
				if (isChar(ESCAPE_CHAR)) {
					buf.push_back(consume());
					buf.push_back(consume());
				}
				buf.push_back(consume());
			}
			discard(); // consume the closing "
			tokens.push_back({ .type = TokenType::string_lit, .value = buf });
			tokens.push_back({ .type = TokenType::closed_Quote });
			buf.clear();
		}else if (isalpha(peak().value())) {
			buf.push_back(consume());
			while (peak().has_value() && isalnum(peak().value())) {
				buf.push_back(consume());
			}
			if (buf == EXIT) {
				tokens.push_back({ .type = TokenType::_exit });
				buf.clear();
			}/*else if (buf == IMPORT) {
				tokens.push_back({ .type = TokenType::_import });
				buf.clear();
			}*/else if (buf == RETURN) {
				tokens.push_back({ .type = TokenType::_return });
				buf.clear();
			}else if (buf == REQUEST) {
				tokens.push_back({ .type = TokenType::request });
				buf.clear();
			}else if (buf == SAY) {
				tokens.push_back({ .type = TokenType::say });
				buf.clear();
			}else if (buf == SHOUT) {
				tokens.push_back({ .type = TokenType::shout });
				buf.clear();
			}else if (buf == IF) {
				tokens.push_back({ .type = TokenType::_if });
				buf.clear();
			}else if (buf == ELSE) {
				tokens.push_back({ .type = TokenType::_else });
				buf.clear();
			}else if (buf == ELIF) {
				tokens.push_back({ .type = TokenType::_elif });
				buf.clear();
			}else if (buf == DUMPVAR) {
				tokens.push_back({ .type = TokenType::var_dump });
				buf.clear();
			}else if (buf == TPPINP) {
				tokens.push_back({ .type = TokenType::tppinp });
				buf.clear();
			}else if (buf == TPPCOUNT) {
				tokens.push_back({ .type = TokenType::tppcount });
				buf.clear();
			}else if (buf == WHILE) {
				tokens.push_back({ .type = TokenType::_while });
				buf.clear();
			}else {
				tokens.push_back({ .type = TokenType::identifier, .value = buf });
				buf.clear();
			}
		}else if (isdigit(peak().value())) {
			buf.push_back(consume());
			while (peak().has_value() && isdigit(peak().value())) {
				buf.push_back(consume());
			}
			tokens.push_back({ .type = TokenType::int_lit, .value = buf });
			buf.clear();
		}else if (isChar(SEMI)) {
			tokens.push_back({ .type = TokenType::semi });
			discard();
		}else if (isChar(OPEN_PAREN)) {
			tokens.push_back({ .type = TokenType::open_Paren });
			discard();
		}else if (isChar(CLOSED_PAREN)) {
			tokens.push_back({ .type = TokenType::closed_Paren });
			discard();
		}else if (isChar(OPEN_CURLY)) {
			tokens.push_back({ .type = TokenType::open_curly });
			discard();
		}else if (isChar(CLOSED_CURLY)) {
			tokens.push_back({ .type = TokenType::closed_curly });
			discard();
		}else if (isChar(EQUAL)) {
			tokens.push_back({ .type = TokenType::equals });
			discard();
		}else if (isChar(SMALLER)) {
			tokens.push_back({ .type = TokenType::test_smaller });
			discard();
		}else if (isChar(LARGER)) {
			tokens.push_back({ .type = TokenType::test_greater });
			discard();
		}else if (isChar(COMMA)) {
			tokens.push_back({ .type = TokenType::comma });
			discard();
		}else if (isChar(ADDITION)) { //TODO pull to types.h
			tokens.push_back({ .type = TokenType::addition });
			discard();
		}else if (isChar(MULTIPLICATION)) { //TODO pull to types.h
			tokens.push_back({ .type = TokenType::multiplication });
			discard();
		}else if (isChar(SUBTRACTION)) { //TODO pull to types.h
			tokens.push_back({ .type = TokenType::subtraction });
			discard();
		}else if (isChar(DIVISION)) { //TODO pull to types.h
			tokens.push_back({ .type = TokenType::division });
			discard();
		}else if (isspace(peak().value())) {
			discard();
		}else {
			cerr << "you messed up with: " << buf << endl;
			exit(EXIT_FAILURE);
		}
	}
	return tokens;
}