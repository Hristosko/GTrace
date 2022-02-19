#pragma once

#include <fstream>
#include <time.h>
#include <mutex>
#include <string_view>
#include "Common.h"

namespace gtrace
{
class Logger
{
public:
    std::mutex mu;
    Logger();
    ~Logger();

    template<typename T>
    friend Logger& operator<<(Logger& logger, const T& val);

    void flush() { file.flush(); }
    static std::string_view fileName(const char* filePath) { return getFileName(filePath); }

private:
    std::ofstream file;
};

template<typename T>
Logger& operator<<(Logger& logger, const T& val)
{
    logger.file << val;
    return logger;
}

Logger& getLogger();
uint64_t getTimestamp();

inline void log()
{
    getLogger() << '\n';
}
template<typename T, typename... Args>
void log(const T& val, const Args&... args)
{
    Logger& logger = getLogger();
    logger << val;
    log(args...);
}

#define LOGSTAT(...)                                                                                                \
    {                                                                                                               \
        std::lock_guard<std::mutex> lock(gtrace::getLogger().mu);                                                   \
        gtrace::log(                                                                                                \
            "[STAT]   ", Logger::fileName(__FILE__), ":", __LINE__, " ", gtrace::getTimestamp(), " ", __VA_ARGS__); \
        gtrace::getLogger().flush();                                                                                \
    }

#define LOGINFO(...)                                                                                                \
    {                                                                                                               \
        std::lock_guard<std::mutex> lock(gtrace::getLogger().mu);                                                   \
        gtrace::log(                                                                                                \
            "[INFO]   ", Logger::fileName(__FILE__), ":", __LINE__, " ", gtrace::getTimestamp(), " ", __VA_ARGS__); \
        gtrace::getLogger().flush();                                                                                \
    }

#define LOGWARNING(...)                                                                                             \
    {                                                                                                               \
        std::lock_guard<std::mutex> lock(gtrace::getLogger().mu);                                                   \
        gtrace::log(                                                                                                \
            "[WARN]   ", Logger::fileName(__FILE__), ":", __LINE__, " ", gtrace::getTimestamp(), " ", __VA_ARGS__); \
        gtrace::getLogger().flush();                                                                                \
    }

#define LOGERROR(...)                                                                                               \
    {                                                                                                               \
        std::lock_guard<std::mutex> lock(gtrace::getLogger().mu);                                                   \
        gtrace::log(                                                                                                \
            "[ERROR]  ", Logger::fileName(__FILE__), ":", __LINE__, " ", gtrace::getTimestamp(), " ", __VA_ARGS__); \
        gtrace::getLogger().flush();                                                                                \
    }

#ifdef _DEBUG
#define LOGDEBUG(...)                                                                                           \
    {                                                                                                           \
        std::lock_guard<std::mutex> lock(gtrace::getLogger().mu);                                               \
        log("[DEBUG]  ", Logger::file(__FILE__), ":", __LINE__, " ", gtrace::getTimestamp(), " ", __VA_ARGS__); \
        gtrace::getLogger().flush();                                                                            \
    }
#else
#define LOGDEBUG(...) ;
#endif

}  // namespace gtrace
