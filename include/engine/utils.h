#pragma once
#include <fstream>
#include <sstream>
#include <string>

#include "engine/logger.h"

namespace utils {
	std::string readFileAbs(const char* path);
}
