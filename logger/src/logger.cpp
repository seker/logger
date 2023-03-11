//
// Created by xinjian
//

#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include <spdlog/sinks/minutely_file_sink.h>

#ifdef __ANDROID__
#include <android/log.h>
#endif

#include "logger.h"
#include "priority.h"

namespace logger {

#define LOG_BUF_SIZE 1024
    static char const *pattern = "%H:%M:%S.%e [%L][tid=%t]%v";

    int gPriority = _VERBOSE_;
    bool gConsole = true;
    std::shared_ptr<spdlog::sinks::minutely_file_sink_mt> minutelyFileSink = nullptr;
    std::shared_ptr<spdlog::logger> gLogger = nullptr;

    extern void before_close(const std::string &filename);

    extern void after_close(const std::string &filename);

    extern void before_open(const std::string &filename);

    extern void after_open(const std::string &filename);

    int init(char const * const rootDir,
                  char const * const baseLogFileName,
                  bool console,
                  int minutes,
                  int priority,
                  std::function<void(const std::string &filename)> before_open,
                  std::function<void(const std::string &filename)> after_open,
                  std::function<void(const std::string &filename)> before_close,
                  std::function<void(const std::string &filename)> after_close) {
        if (!gLogger) {
            std::string logFileName = rootDir;
            if ('/' != logFileName.at(logFileName.length() - 1)) {
                logFileName.append("/");
            }
            logFileName.append(baseLogFileName).append(".log");

            gConsole = console;
            gPriority = priority;

            spdlog::file_event_handlers my_file_event_handlers = spdlog::file_event_handlers();
            my_file_event_handlers.before_open = [&](const spdlog::filename_t &filename) {
                before_open(filename);
            };
            my_file_event_handlers.after_open = [&](const spdlog::filename_t &filename, std::FILE *file_stream) {
                if (spdlog::details::os::filesize(file_stream) > 0) {
                    fputs("\n\n", file_stream);
                }
                after_open(filename);
            };
            my_file_event_handlers.before_close = [&](const spdlog::filename_t &filename, std::FILE *file_stream) {
                before_close(filename);
            };
            my_file_event_handlers.after_close = [&](const spdlog::filename_t &filename) {
                after_close(filename);
            };
            minutelyFileSink = std::make_shared<spdlog::sinks::minutely_file_sink_mt>(logFileName, false, minutes,
                                                                                           0,
                                                                                           my_file_event_handlers);
            minutelyFileSink->set_pattern(pattern);
            gLogger = std::make_shared<spdlog::logger>("logger", minutelyFileSink);
            gLogger->set_level(spdlog::level::trace);

            gLogger->info("init logger : ============================ init logger ============================");
            gLogger->info("init logger : rootDir         = {}", rootDir);
            gLogger->info("init logger : baseLogFileName = {}", baseLogFileName);
            gLogger->info("init logger : console         = {}", B2S(gConsole));
            gLogger->info("init logger : minutes         = {}", minutes);
            gLogger->info("init logger : logFileName     = {}", logFileName);
            return 0;
        }
        return -1;
    }

    int setConsole(bool console)
    {
        gConsole = console;
        return 0;
    }

    int setPriority(int priority)
    {
        gPriority = priority;
        return 0;
    }

    int setMinutes(int minutes)
    {
        if (minutelyFileSink)
        {
            minutelyFileSink->set_minutes(minutes);
            return 0;
        }
        return -1;
    }

    int log(int priority, const char* const tag, const char* const threadName, const char* const msg) {
        if (gPriority > priority) {
            return -1;
        }

        if (!gConsole && !gLogger) {
            return -2;
        }

        int ret = 0;
        if (gConsole) {
#ifdef __ANDROID__
            size_t len_tag = 5 + strlen(threadName);
            size_t len_msg = strlen(msg);
            const size_t step = LOG_BUF_SIZE - 1 - len_tag;
            if (len_msg > step) {
                std::string message = std::string (msg);
                size_t index = 0;
                while (index < len_msg) {
                    size_t nextIndex = std::min(index + step, len_msg);
                    ret = __android_log_print(priority, tag, "[%s] : %s", threadName, message.substr(index, nextIndex).c_str());
                    index = nextIndex;
                }
            } else {
                ret = __android_log_print(priority, tag, "[%s] : %s", threadName, msg);
            }
#endif
        }

        if (gLogger) {
            spdlog::level::level_enum lvl = level(priority);
            if (spdlog::level::off == lvl) {
                ret = -3;
            } else {
                gLogger->log(lvl, "[{}][{}] : {}", threadName, tag, msg);
                ret = 0;
            }
        }

        return ret;
    }

    // https://mp.weixin.qq.com/s?__biz=MzU4OTQyODI0Mw==&mid=2247484310&idx=1&sn=1df96021d9bcdbc3fbedcf8b333befd6&chksm=fdcce0eacabb69fc3061611ba087ee0192209fa8301814974111bc4f1aaef60eb72184ee6787&scene=27
    int log(int priority, const char* const tag, const char* const fmt, ...) {
        if (gPriority > priority) {
            return -1;
        }

        if (!gConsole && !gLogger) {
            return -2;
        }

        va_list args;
        va_start(args, fmt);
        const auto len = vsnprintf(nullptr, 0, fmt, args);
        va_end(args);

        std::string msg;
        msg.resize(static_cast<size_t>(len) + 1);
        va_start(args, fmt);
        vsnprintf(&msg.front(), len + 1, fmt, args);
        va_end(args);
        msg.resize(static_cast<size_t>(len));

        return log(priority, tag, "unknown", msg.c_str());
    }

    int flush() {
        if (gLogger) {
            gLogger->flush();
            return 0;
        }
        return -1;
    }
}