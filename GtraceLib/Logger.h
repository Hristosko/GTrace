#pragma once

#include <fstream>
#include <time.h>
#include<mutex>

namespace gtrace {

class Logger {
public:
	std::mutex mu;
	Logger();
	~Logger();

	template<typename T>
	friend Logger& operator<<(Logger& logger, const T& val);

	void flush() { this->file.flush(); }
private:
	std::ofstream file;
};

template<typename T>
Logger& operator<<(Logger& logger, const T& val) {
	logger.file << val;
	return logger;
}

Logger& getLogger();
uint64_t getTimestamp();

template<typename T, typename... Args>
void log(const T& val, const Args& ... args) {
	Logger& logger = getLogger();
	logger << val;
	log(args...);
}
inline void log() { getLogger() << '\n'; }

#define LOGSTAT(...) \
	{ \
		std::lock_guard<std::mutex> lock(gtrace::getLogger().mu); \
		gtrace::log("[STAT]  ", __FILE__, ":", __LINE__, " ", gtrace::getTimestamp(), " ", __VA_ARGS__); \
		gtrace::getLogger().flush(); \
	}

#define LOGINFO(...) \
	{ \
		std::lock_guard<std::mutex> lock(gtrace::getLogger().mu); \
		gtrace::log("[INFO]  ", __FILE__, ":", __LINE__, " ", gtrace::getTimestamp(), " ", __VA_ARGS__); \
		gtrace::getLogger().flush(); \
	}

#define LOGWARNING(...) \
	{ \
		std::lock_guard<std::mutex> lock(gtrace::getLogger().mu); \
		gtrace::log("[WARN]  ", __FILE__, ":", __LINE__, " ", gtrace::getTimestamp(), " ", __VA_ARGS__); \
		gtrace::getLogger().flush(); \
	}

#define LOGERROR(...) \
	{ \
		std::lock_guard<std::mutex> lock(gtrace::getLogger().mu); \
		gtrace::log("[ERROR]  ", __FILE__, ":", __LINE__, " ", gtrace::getTimestamp(), " ", __VA_ARGS__); \
		gtrace::getLogger().flush(); \
	}

#ifdef _DEBUG
#define LOGDEBUG(...) \
	{ \
		std::lock_guard<std::mutex> lock(gtrace::getLogger().mu); \
		log("[DEBUG]  ", __FILE__, ":", __LINE__, " ", gtrace::getTimestamp(), " ", __VA_ARGS__); \
		gtrace::getLogger().flush(); \
	}
#else
#define LOGDEBUG(...) ;
#endif

}
