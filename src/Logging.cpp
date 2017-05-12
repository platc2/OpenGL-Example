#include "Logging.h"
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#ifdef _WIN32
#include <Windows.h>
#endif

using namespace std;


#ifdef _WIN32
const static HANDLE xOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
const static int COLOR_BG_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
const static int COLOR_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const static int COLOR_BLACK = 0;
const static int COLOR_DARKGRAY = COLOR_BLACK | FOREGROUND_INTENSITY;
const static int COLOR_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const static int COLOR_BROWN = FOREGROUND_RED | FOREGROUND_GREEN;
const static int COLOR_LIGHTRED = FOREGROUND_RED | FOREGROUND_INTENSITY;
const static int COLOR_DARKRED = FOREGROUND_RED;
#endif


#ifdef __linux
void log(FILE* pFD, const char* szPrefix, const char* szFormat, va_list& xList)
{
	fprintf(pFD, "\033[37m[%s\033[37m]\033[0m ", szPrefix);
	vfprintf(pFD, szFormat, xList);
	fprintf(pFD, "\n");
}
#elif _WIN32
void log(const int iColor, const char* szPrefix, const char* szFormat, va_list& xList)
{
	SetConsoleTextAttribute(xOutputHandle, COLOR_BG_WHITE | COLOR_DARKGRAY); fprintf(stdout, "[");
	SetConsoleTextAttribute(xOutputHandle, COLOR_BG_WHITE | iColor); printf("%s", szPrefix);
	SetConsoleTextAttribute(xOutputHandle, COLOR_BG_WHITE | COLOR_DARKGRAY); printf("] ");
	SetConsoleTextAttribute(xOutputHandle, COLOR_BG_WHITE | COLOR_BLACK); vprintf(szFormat, xList);
	printf("\n");
}
#endif


void logMessage(const char* szFormat, ...)
{
	va_list xList;
	va_start(xList, szFormat);

#ifdef __linux
	log(stdout, "\033[34mMSG", szFormat, xList);
#elif _WIN32
	log(COLOR_BLUE, "MSG", szFormat, xList);
#endif

	va_end(xList);
}


void logWarn(const char* szFormat, ...)
{
	va_list xList;
	va_start(xList, szFormat);

#ifdef __linux
	log(stderr, "\033[93mWRN", szFormat, xList);
#elif _WIN32
	log(COLOR_BROWN, "WRN", szFormat, xList);
#endif

	va_end(xList);
}


void logError(const char* szFormat, ...)
{
	va_list xList;
	va_start(xList, szFormat);

#ifdef __linux
	log(stderr, "\033[91mERR", szFormat, xList);
#elif _WIN32
	log(COLOR_LIGHTRED, "ERR", szFormat, xList);
#endif

	va_end(xList);
}


void logFatal(const char* szFormat, ...)
{
	va_list xList;
	va_start(xList, szFormat);

#ifdef __linux
	log(stderr, "\033[91mFAT", szFormat, xList);
#elif _WIN32
	log(COLOR_DARKRED, "FAT", szFormat, xList);
#endif

	va_end(xList);
}

