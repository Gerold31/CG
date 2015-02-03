#ifndef LOGGING
#define LOGGING

#include <cstdio>
#include <iostream>


#ifdef LOG_COLOR
#define LOG_COLOR_RESET "\033[0m"
#define LOG_COLOR_WARN "\033[0;33m"
#define LOG_COLOR_SEVE "\033[0;31m"
#else
#define LOG_COLOR_RESET ""
#define LOG_COLOR_WARN ""
#define LOG_COLOR_SEVE ""
#endif

template <typename... Args>
static void PRINT(const char *format, Args... args)
{
	char buf_[2048];
	snprintf(buf_, 2048, format, args...);
	std::cerr << buf_ << LOG_COLOR_RESET << std::endl;
}

#if defined(LOG_FINE)
#define FINE(msg...) PRINT("FINE: " msg)
#else
#define FINE(msg...)
#endif

#if defined(LOG_FINE) || defined(LOG_INFO)
#define INFO(msg...) PRINT("INFO: " msg)
#else
#define INFO(msg...)
#endif

#if defined(LOG_FINE) || defined(LOG_INFO) || defined(LOG_WARNING)
#define WARNING(msg...) PRINT(LOG_COLOR_WARN "WARN: " msg)
#else
#define WARNING(msg...)
#endif

#if defined(LOG_FINE) || defined(LOG_INFO) || defined(LOG_WARNING) || defined(LOG_SEVERE)
#define SEVERE(msg...) PRINT(LOG_COLOR_SEVE "SEVE: " msg)
#else
#define SEVERE(msg...)
#endif

#endif // LOGGING
