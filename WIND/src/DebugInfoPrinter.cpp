#include "DebugInfoPrinter.h"


WIND::UInt32 DebugInfoPrinter::s_verbosity = 0;



WIND::UInt32 DebugInfoPrinter::DebugPrintF(const char *format, ...)
{
	va_list argList;
	va_start (argList, format);
	WIND::Int32 charsWritten = VDebugPrintF(format, argList);
	va_end (argList);

	return charsWritten;
}



WIND::UInt32 DebugInfoPrinter::VDebugPrintF(const char *format, va_list argList)
{
	const WIND::UInt32 MAX_CHARS = 1023;
	static char s_buffer[MAX_CHARS + 1];
	WIND::Int32 charsWritten = vsnprintf(s_buffer, MAX_CHARS, format, argList);
	s_buffer[MAX_CHARS] = '\0';

	OutputDebugString (s_buffer);
	return charsWritten;
}



WIND::UInt32 DebugInfoPrinter::OutputToConsole(WIND::UInt32 verbosity, const char *format, ...)
{
	if (GetVerbosity() >= verbosity) {
		va_list argList;
		va_start (argList, format);
		WIND::Int32 charsWritten = VDebugPrintF(format, argList);
		va_end (argList);

		return charsWritten;
	}

	return 0;
}



WIND::UInt32 DebugInfoPrinter::OutputToConsoleAndLog (WIND::UInt32 verbosity, const char *format, ...)
{
	if (GetVerbosity() >= verbosity) {
		va_list argList;
		va_start (argList, format);
		WIND::Int32 charsWritten = VDebugPrintF(format, argList);
		va_end (argList);

		// –¥»’÷æ

		return charsWritten;
	}
	return 0;
}