#include "FileUtils.h"
#include "Errors.h"
#include "Logger.h"

void readBin(void* buffer, size_t elementSize, size_t elementCount, FILE* fp) {
	if (fp == nullptr) {
		LOGERROR("Attempting to read from not existing file.");
		throw FileReadError();
	}
	const size_t dataSize = elementCount * elementSize;
	const size_t res = fread(buffer, elementSize, elementCount, fp);
	if (res != dataSize) {
		LOGERROR("Unable to read all requested data from file.");
		throw FileReadError();
	}
}

void writeBin(void* buffer, size_t elementSize, size_t elementCount, FILE* fp) {
	if (fp == nullptr) {
		LOGERROR("Attempting to write to not existing file.");
		throw FileWriteError();
	}
	const size_t dataSize = elementCount * elementSize;
	const size_t res = fwrite(buffer, elementSize, elementCount, fp);
	if (res != dataSize) {
		LOGERROR("Unable to write all requested data to file.");
		throw FileWriteError();
	}
}
