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
		log("[INFO]  ", __FILE__, ":", __LINE__, " ", getTimestamp(), __VA_ARGS__, '\n', '\n'); \
	}

#define LOGERROR(...) \
	{ \
		log("[ERROR]  ", __FILE__, ":", __LINE__, " ", getTimestamp(), __VA_ARGS__, '\n', '\n'); \
	}

#ifdef _DEBUG
#define LOGDEBUG(...) \
	{ \
		log("[DEBUG]  ", __FILE__, ":", __LINE__, " ", getTimestamp(), __VA_ARGS__, '\n', '\n'); \
	}
#else 
#define LOGDEBUG(...) ;
#endif
