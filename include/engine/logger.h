#pragma once
#include <iostream>
#include <ctime>

#ifdef __linux__
	#define CON_RED "\033[31m"
	#define CON_GREEN "\033[32m"
	#define CON_YELLOW "\033[33m"
	#define CON_WHITE "\033[0m"
	#define CON_GRAY "\033[2m"
#endif

class Logger {
private:
	static Logger& instance;
	Logger();

	int m_logLevel = INFO;

public:
	static const int ERR = 0;
	static const int WARN = 1;
	static const int INFO = 2;
	static const int DETAIL = 3;

	static Logger& getInstance();
	
	void setLogLevel(int level);
	int  getLogLevel() const;

	void detail(const char* prefix, const char* msg) const;
	void info  (const char* prefix, const char* msg) const;
	void warn  (const char* prefix, const char* msg) const;
	void err   (const char* prefix, const char* msg) const;
};
