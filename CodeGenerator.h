#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include "Types.h"

std::string generateCCode(const std::vector<Token>& tokens);