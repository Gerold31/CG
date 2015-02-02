#ifndef LOGGING
#define LOGGING

#include <cstdio>
#include <iostream>


#define PRINT(prefix, msg...) \
{ \
	char buf_[2048]; \
	snprintf(buf_, 2048, prefix msg); \
	std::cerr << buf_ << std::endl; \
}

#if defined(LOG_FINE)
#define FINE(msg...) PRINT("fine: ", msg)
#else
#define FINE(msg...)
#endif

#if defined(LOG_FINE) || defined(LOG_INFO)
#define INFO(msg...) PRINT("info: ", msg)
#else
#define INFO(msg...)
#endif

#if defined(LOG_FINE) || defined(LOG_INFO) || defined(LOG_WARNING)
#define WARNING(msg...) PRINT("warning: ", msg)
#else
#define WARNING(msg...)
#endif

#if defined(LOG_FINE) || defined(LOG_INFO) || defined(LOG_WARNING) || defined(LOG_SEVERE)
#define SEVERE(msg...) PRINT("severe: ", msg)
#else
#define SEVERE(msg...)
#endif

#endif // LOGGING
