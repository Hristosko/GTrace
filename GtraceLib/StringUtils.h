#pragma once

#include <string>

inline bool endsWith(const std::string& str, const std::string& ending) {
	if (ending.size() > str.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
}

inline bool startsWith(const std::string& str, const std::string& ending) {
	if (ending.size() > str.size()) return false;
	return std::equal(ending.begin(), ending.end(), str.begin());
}