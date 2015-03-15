#pragma once
#include "DataType.h"
#include <stdio.h> // 为了va_list
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <Windows.h> // 为了OutputDebugString()

class DebugInfoPrinter
{
public:
	DebugInfoPrinter () {}
	~DebugInfoPrinter() {}

public:
	WIND::UInt32 OutputToConsole (WIND::UInt32 verbosity, const char *format, ...);
	WIND::UInt32 OutputToConsoleAndLog (WIND::UInt32 verbosity, const char *format, ...);

public:
	static void SetVerbosity (WIND::UInt32 verb) { s_verbosity = verb; }
	static WIND::UInt32 GetVerbosity () { return s_verbosity; }

private:
	WIND::UInt32 VDebugPrintF (const char *format, va_list argList);
	WIND::UInt32 DebugPrintF (const char* format, ...);

private:
	static WIND::UInt32 s_verbosity; // 冗长级别
};