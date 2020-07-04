#include "Logger.h"

#define LOG_FILE_PATH "gtrace.log"

Logger::Logger()
	: file(LOG_FILE_PATH, std::ios::out | std::ios::trunc) {}


Logger::~Logger() {
	this->file.close();
}

Logger& getLogger() {
	static Logger logger;
	return logger;
}

std::string getTimestamp() {
	char buffer[512] = { '\0' };
	time_t now = time(&now);
	struct tm local_time;
	localtime_s(&local_time, &now);
	strftime(buffer, BUFSIZ, "%m/%d/%Y %H:%M:%S ", &local_time);
	return std::string(buffer);
}
