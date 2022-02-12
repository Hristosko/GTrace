#include "Logger.h"
#include <chrono>

#define LOG_FILE_PATH "gtrace.log"

namespace gtrace {

Logger::Logger()
    : file(LOG_FILE_PATH, std::ios::out | std::ios::trunc) {}


Logger::~Logger() {
    this->file.close();
}

Logger& getLogger() {
    static Logger logger;
    return logger;
}

uint64_t getTimestamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
        ).count();
}
}
