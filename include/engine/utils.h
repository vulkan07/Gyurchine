#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "engine/logger.h"
#include "engine/utils.h"

namespace utils {
	std::string readFile(const char* pathc, const bool relative);
}
