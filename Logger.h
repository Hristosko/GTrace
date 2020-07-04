#pragma once

#include <fstream>
#include <time.h>

class Logger {
public:
	Logger();
	~Logger();

	template<typename T>
	friend Logger& operator<<(Logger& logger, const T& val);
private:
	std::ofstream file;
};

template<typename T>
Logger& operator<<(Logger& logger, const T& val) {
	logger.file << val;
	return logger;
}

Logger& getLogger();
std::string getTimestamp();

template<typename T, typename... Args>
void log(const T& val, const Args& ... args) {
	Logger& logger = getLogger();
	logger << val;
	log(args...);
}
inline void log() { getLogger() << '\n'; }

#define LOGINFO(...) \
	{ \
		log("[INFO]  ", getTimestamp(), __VA_ARGS__, '\n'); \
	}

#define LOGERROR(...) \
	{ \
		log("[ERROR]  ", getTimestamp(), __VA_ARGS__, '\n'); \
	}

#ifdef _DEBUG
#define LOGDEBUG(...) \
	{ \
		log("[DEBUG]  ", getTimestamp(), __VA_ARGS__, '\n'); \
	}
#else 
#define LOGDEBUG(...) ;
#endif
