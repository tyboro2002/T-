#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include "Types.h"
#include "Parser.h"

class Generator {
	program m_prog;
public:
	Generator(program root) :m_prog(std::move(root)) {}
	[[nodiscard]] std::string generate() const;
private:
};