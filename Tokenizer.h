#pragma once
#include "Types.h"

#include <vector>

class Tokenizer {
	const std::string m_src; //	the input code
	int m_index; // index in the input code
public:
	inline Tokenizer(std::string src) : m_src(src), m_index(0) {}
	/* convert the src to a vector of Tokens */
	std::vector<Token> tokenize();
private:
	/* peak <ahead> characters ahead in the src (dont consume) */
	[[nodiscard]] std::optional<char> peak(int ahead = 1) const;
	/* consume 1 char in the src and return it */
	char consume();
	/* discard <amount> chars from the src (dont return them) */
	void discard(int amount = 1);
	/* try consuming <karakter> is found return <karakter> else exit the program with <errorMessage> */
	char tryConsume(char karakter, std::string errorMessage);
	/* test if in the src <ahead> characters ahead is a <karakter> */
	bool isChar(char karakter, int ahead = 1);
	/* test if in the src <ahead> characters ahead is a <karakter> (also returns true if the src ends) */
	bool isNotChar(char karakter, int ahead = 1);
};