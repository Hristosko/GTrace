#include "FileUtils.h"
#include "Errors.h"
#include "Logger.h"

void readBin(void* buffer, size_t elementSize, size_t elementCount, FILE* fp) {
	if (fp == nullptr) {
		LOGERROR("Attempting to read from not existing file.");
		throw FileReadError();
	}
	const size_t res = fread(buffer, elementSize, elementCount, fp);
	if (res != elementCount) {
		LOGERROR("Unable to read all requested data from file: ", res, " from ", elementCount);
		throw FileReadError();
	}
}

void writeBin(const void* buffer, size_t elementSize, size_t elementCount, FILE* fp) {
	if (fp == nullptr) {
		LOGERROR("Attempting to write to not existing file.");
		throw FileWriteError();
	}
	const size_t res = fwrite(buffer, elementSize, elementCount, fp);
	if (res != elementCount) {
		LOGERROR("Unable to write all requested data to file: ", res, " from ", elementCount);
		throw FileWriteError();
	}
}

int readLine(char* buffer, int n, FILE* fp) {
	char* res = fgets(buffer, n, fp);
	if (res == 0) return -1;
	size_t size = strnlen_s(buffer, n);
	if (size == n && buffer[n - 1] != '\0') {
		LOGERROR("Too long line. Maximum length is: ", n);
		throw FileWriteError();
	}
	// remove the new line symbol
	if (res[size - 1] == '\n') res[--size] = '\0';
	return size;
}
