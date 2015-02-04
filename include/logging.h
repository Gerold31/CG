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

#define PRINT(format, ...) \
{ \
	char buf_[2048]; \
	snprintf(buf_, 2048, format LOG_COLOR_RESET, ##__VA_ARGS__); \
	std::cerr << buf_ << std::endl; \
}

#if defined(LOG_FINE)
#define FINE(...) PRINT("FINE: " __VA_ARGS__)
#else
#define FINE(...) ((void)0)
#endif

#if defined(LOG_FINE) || defined(LOG_INFO)
#define INFO(...) PRINT("INFO: " __VA_ARGS__)
#else
#define INFO(...) ((void)0)
#endif

#if defined(LOG_FINE) || defined(LOG_INFO) || defined(LOG_WARNING)
#define WARNING(...) PRINT(LOG_COLOR_WARN "WARN: " __VA_ARGS__)
#else
#define WARNING(...) ((void)0)
#endif

#if defined(LOG_FINE) || defined(LOG_INFO) || defined(LOG_WARNING) || defined(LOG_SEVERE)
#define SEVERE(...) PRINT(LOG_COLOR_SEVE "SEVE: " __VA_ARGS__)
#else
#define SEVERE(...) ((void)0)
#endif

#endif // LOGGING
