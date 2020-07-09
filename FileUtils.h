#pragma once

#include <stdio.h>

struct FileRAII {
	FILE* fp;
	FileRAII(FILE* fp) : fp(fp) {}
	FileRAII(const FileRAII&) = delete;
	FileRAII& operator=(const FileRAII&) = delete;
	~FileRAII() {
		if (fp != nullptr) fclose(fp);
	}
};


void readBin(void* buffer, size_t elementSize, size_t elementCount, FILE* fp);
void writeBin(void* buffer, size_t elementSize, size_t elementCount, FILE* fp);