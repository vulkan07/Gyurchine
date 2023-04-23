#include "engine/utils.h"

namespace utils {

	//// Common use for simple text file read
	//// Returns "" when file could not be read
	std::string readFileAbs(const char* path) {
		std::ifstream f(path);
		std::string s = "";
		if (f) {
			std::ostringstream ss;
			ss << f.rdbuf();
			s = ss.str();
		} else {
			std::string msg = "Couldn't read file ";
			msg += path;
			Logger::getInstance().err("FILEIO",msg.c_str());
		}
		return s;
	}
}
