//
// Created by xinjian
//

#ifndef LOGGER_H
#define LOGGER_H

// 不能删，strrchr() 需要引入它
#include <string>

#include "defs.h"

namespace logger
{

    const int _VERBOSE_             = 2;
    const int _DEBUG_               = 3;
    const int _INFO_                = 4;
    const int _WARN_                = 5;
    const int _ERROR_               = 6;
    const int _FATAL_               = 7;

    EXPORT int init(char const * const rootDir,
                 char const * const baseLogFileName = "log",
                 bool console = true,
                 int minutes = 20,
                 int priority = _VERBOSE_,
                 std::function<void(const std::string &filename)> before_open = nullptr,
                 std::function<void(const std::string &filename, std::FILE *file_stream)> after_open = nullptr,
                 std::function<void(const std::string &filename, std::FILE *file_stream)> before_close = nullptr,
                 std::function<void(const std::string &filename)> after_close = nullptr);

    EXPORT int setConsole(bool console);

    EXPORT int setMinutes(int minutes);

    EXPORT int setPriority(int priority);

    EXPORT int log(int priority, const char* const tag, const char* const threadName, const char* const msg);
    EXPORT int log(int priority, const char* const tag, const char* const fmt, ...);

    EXPORT int flush();
}

#ifdef _WIN32
#define __FILENAME__ (strrchr("\\" __FILE__, '\\') + 1)
#else
#define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)
#endif

#define LOG(priority, tag, FMT, ...)        \
    logger::log(priority, tag,              \
    "[%s:%d] : " FMT, __FILENAME__, __LINE__, ##__VA_ARGS__)

#define LOGV(tag, FMT, ...)                 \
    logger::log(logger::_VERBOSE_, tag,     \
    "[%s:%d] : " FMT, __FILENAME__, __LINE__, ##__VA_ARGS__)

#define LOGD(tag, FMT, ...)                 \
    logger::log(logger::_DEBUG_, tag,       \
    "[%s:%d] : " FMT, __FILENAME__, __LINE__, ##__VA_ARGS__)

#define LOGI(tag, FMT, ...)                 \
    logger::log(logger::_INFO_, tag,        \
    "[%s:%d] : " FMT, __FILENAME__, __LINE__, ##__VA_ARGS__)

#define LOGW(tag, FMT, ...)                 \
    logger::log(logger::_WARN_, tag,        \
    "[%s:%d] : " FMT, __FILENAME__, __LINE__, ##__VA_ARGS__)

#define LOGE(tag, FMT, ...)                 \
    logger::log(logger::_ERROR_, tag,       \
    "[%s:%d] : " FMT, __FILENAME__, __LINE__, ##__VA_ARGS__)

#define LOGF(tag, FMT, ...)                 \
    logger::log(logger::_FATAL_,   tag,     \
    "[%s:%d] : " FMT, __FILENAME__, __LINE__, ##__VA_ARGS__)

#endif //LOGGER_H
