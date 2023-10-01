#pragma once
#include "Types.h"

#include <vector>

class Tokenizer {
	const std::string m_src;
	int m_index;
public:
	inline Tokenizer(std::string src) : m_src(src), m_index(0) {

	}
	std::vector<Token> tokenize();
private:
	[[nodiscard]] std::optional<char> peak(int ahead = 1) const;
	char consume();
};