#pragma once

#include <stdio.h>

namespace gtrace {

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
void writeBin(const void* buffer, size_t elementSize, size_t elementCount, FILE* fp);
int readLine(char* buffer, int n, FILE* fp);
}
