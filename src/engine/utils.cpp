#include "engine/utils.h"
#include "engine/game.h"

namespace utils {

	//// Common use for simple text file read
	//// Returns "" when file could not be read
	std::string readFile(const char* pathc, const bool relative) {
		std::string path;
		
		if (relative)
			path += Game::getGameDir() + pathc;
		else
			path = pathc;

		Logger::getInstance().warn("FILE READ",path.c_str());
		
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
