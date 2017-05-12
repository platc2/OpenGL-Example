#ifndef LOGGING_H
#define LOGGING_H

#include <ostream>
#include <sstream>


void logMessage(const char* szFormat, ...);


void logWarn(const char* szFormat, ...);


void logError(const char* szFormat, ...);


void logFatal(const char* szFormat, ...);


#endif

