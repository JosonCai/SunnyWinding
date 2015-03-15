#pragma once
#include <string>

// Ìæ»»×Ö·û´®
void ReplaceString (std::string &strBase, const std::string &srcSrc, const std::string &strDst);
template<typename T>
void SafeDelete (T* pointer) { if (pointer != NULL) {delete pointer; pointer = NULL; }}