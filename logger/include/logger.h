//
// Created by hector on 2021/7/7.
//

#ifndef TUTORIAL_CPP_COMMON_LOGGER_H
#define TUTORIAL_CPP_COMMON_LOGGER_H

#include <cstddef>
#include <cstdint>
#include <cstdarg>

#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace logger {

const int _VERBOSE_             = 0;
const int _DEBUG_               = 1;
const int _INFO_                = 2;
const int _WARN_                = 3;
const int _ERROR_               = 4;
const int _FATAL_               = 5;
const int _OFF_                 = 6;

typedef void (*log_file_rotate_callback_t)(char const * preLogFile, char const * nextLogFile);

EXPORT void init(bool console, char const * rootDir, char const * processName, uint8_t minutes = 10, uint16_t maxFileCount = 72);

EXPORT void set_log_level(int lvl);

EXPORT void set_log_file_rotate_callback(log_file_rotate_callback_t cb);

EXPORT int log(int priority, char const * fmt, va_list args);

EXPORT int log(int priority, char const * fmt, ...);

}

#define InitLog(console, rootDir, processName, minutes, maxFileCount)      \
   logger::init(console, rootDir, processName, minutes, maxFileCount)

#ifdef _WIN32
#define __FILENAME__ (strrchr("\\" __FILE__, '\\') + 1)
#else
#define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)
#endif

#define LOG(priority, tag, FMT, ...)        \
    logger::log(priority,                   \
    "[%s][%s:%d] : " FMT, tag, __FILENAME__, __LINE__, ##__VA_ARGS__)

#define LOGV(tag, FMT, ...)                 \
    logger::log(logger::_VERBOSE_,          \
    "[%s][%s:%d] : " FMT, tag, __FILENAME__, __LINE__, ##__VA_ARGS__)

#define LOGD(tag, FMT, ...)                 \
    logger::log(logger::_DEBUG_,            \
    "[%s][%s:%d] : " FMT, tag, __FILENAME__, __LINE__, ##__VA_ARGS__)

#define LOGI(tag, FMT, ...)                 \
    logger::log(logger::_INFO_,             \
    "[%s][%s:%d] : " FMT, tag, __FILENAME__, __LINE__, ##__VA_ARGS__)

#define LOGW(tag, FMT, ...)                 \
    logger::log(logger::_WARN_,             \
    "[%s][%s:%d] : " FMT, tag, __FILENAME__, __LINE__, ##__VA_ARGS__)

#define LOGE(tag, FMT, ...)                 \
    logger::log(logger::_ERROR_,            \
    "[%s][%s:%d] : " FMT, tag, __FILENAME__, __LINE__, ##__VA_ARGS__)

#define LOGF(tag, FMT, ...)                 \
    logger::log(logger::_FATAL_,            \
    "[%s][%s:%d] : " FMT, tag, __FILENAME__, __LINE__, ##__VA_ARGS__)

#define P2S(p)              (p ? "!nullptr" : "nullptr")
#define B2S(b)              (b ? "true" : "false")

#endif //TUTORIAL_CPP_COMMON_LOGGER_H
