#include "engine/logger.h"

Logger& Logger::instance = Logger::getInstance();
Logger::Logger() {detail("LOGGER", "Logger started");}
Logger& Logger::getInstance() {
	static Logger instance;
	return instance;
}

	
void Logger::setLogLevel(int level) {m_logLevel = std::max(0,std::min(level,4));}
int  Logger::getLogLevel() const {return m_logLevel;}

static void getTimeString(char* buffer, size_t size, const char* format) {
    std::time_t now = std::time(nullptr);
    std::strftime(buffer, size, format, std::localtime(&now));
}

void Logger::err(const char* prefix, const char* msg) const {
	char buffer[9];
	getTimeString(buffer, sizeof(buffer), "%H:%M:%S");
	std::cout << CON_RED << "[" << buffer << "]" << "[" << prefix << "] " << msg << CON_WHITE << std::endl;
}

void Logger::warn(const char* prefix, const char* msg) const {
	if (m_logLevel < 1) return;
	char buffer[9];
	getTimeString(buffer, sizeof(buffer), "%H:%M:%S");
	std::cout << CON_YELLOW << "[" << buffer << "]" << "[" << prefix << "] " << msg << CON_WHITE << std::endl;
}

void Logger::info(const char* prefix, const char* msg) const {
	if (m_logLevel < 2) return;
	char buffer[9];
	getTimeString(buffer, sizeof(buffer), "%H:%M:%S");
	std::cout << "[" << buffer << "]" << "[" << prefix << "] " << msg << std::endl;
}

void Logger::detail(const char* prefix, const char* msg) const {
	if (m_logLevel < 3) return;
	char buffer[9];
	getTimeString(buffer, sizeof(buffer), "%H:%M:%S");
	std::cout << CON_GRAY << "[" << buffer << "]" << "[" << prefix << "] " << msg << CON_WHITE << std::endl;
}

