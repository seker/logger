//
// Created by hector on 2021/7/7.
//

#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/minutely_file_sink.h>


#include <cstring>

#include "logger.h"

/* va_copy is a C99 feature. In C89 implementations, it's sometimes
   available as __va_copy. If not, memcpy() should do the trick. */
#ifndef va_copy
#ifdef __va_copy
#define va_copy __va_copy
#else
#define va_copy(a, b)  memcpy(&(a), &(b), sizeof(va_list))
#endif
#endif

namespace logger {

static const size_t LOG_MAX_LENGTH   = 2048;

struct my_file_event_handlers : public spdlog::file_event_handlers
{
    std::string preLogFile;
    log_file_rotate_callback_t cb_;

    my_file_event_handlers()
    {
        after_close = [this](const spdlog::filename_t& filename) 
        {
            preLogFile.clear();
            preLogFile.append(filename);
        };

        before_open = [this](const spdlog::filename_t& filename) 
        {
            if (cb_) 
            {
                cb_(preLogFile.c_str(), filename.c_str());
            }
        };

        after_open = [this](const spdlog::filename_t& filename, std::FILE *fstream) 
        {
            if (spdlog::details::os::filesize(fstream) > 0) 
            {
                fputs("\n", fstream);
            }
        };
    }
} my_file_event_handlers_;

spdlog::logger * gLogger = nullptr;

static char const * pattern = "%H:%M:%S.%e [%L][tid=%t]%v";

#ifdef _WIN32
#include <windows.h>
std::string UTF8_To_string(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];    //一定要加1，不然会出现尾巴 
	memset(pwBuf, 0, nwLen * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string strRet = pBuf;

	delete[]pBuf;
	delete[]pwBuf;
	pBuf = NULL;
	pwBuf = NULL;

	return strRet;
}
// std:string转UTF8
std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];    //一定要加1，不然会出现尾巴 
	ZeroMemory(pwBuf, nwLen * 2 + 2);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);
	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string strRet(pBuf);

	delete[]pwBuf;
	delete[]pBuf;
	pwBuf = NULL;
	pBuf = NULL;

	return strRet;
}
#endif // _WIN32

void init(bool console, char const * rootDir, char const * processName, uint8_t minutes, uint16_t maxFileCount) {
    if (!gLogger) {
        
#ifdef _WIN32
        std::string logFileName = UTF8_To_string(rootDir);
        if ('\\' != logFileName.at(logFileName.length() - 1))
        {
            logFileName.append("\\");
        }
#else
        std::string logFileName = rootDir;
        if ('/' != logFileName.at(logFileName.length() - 1))
        {
            logFileName.append("/");
        }
#endif
        logFileName.append(processName).append(".log");

        spdlog::info("init processName={0}, rootDir={1}, logFileName={2}", processName, rootDir, logFileName.c_str());
        
        auto minutely_logger = std::make_shared<spdlog::sinks::minutely_file_sink_mt>(logFileName.c_str(), false, minutes, maxFileCount, my_file_event_handlers_);
        minutely_logger->set_pattern(pattern);
        minutely_logger->set_level(spdlog::level::level_enum::debug);

        if (console) {
            auto console_logger = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_logger->set_pattern(pattern);
            console_logger->set_level(spdlog::level::level_enum::trace);

            gLogger = new spdlog::logger("logger", {minutely_logger, console_logger});
        } else {
            gLogger = new spdlog::logger("logger", minutely_logger);
        }
        gLogger->set_level(spdlog::level::level_enum::trace);

        gLogger->info("init logger : ============================ init logger ============================");
        gLogger->info("init logger : console         = {}", B2S(console));
        gLogger->info("init logger : rootDir         = {}", rootDir);
        gLogger->info("init logger : processName     = {}", processName);
        gLogger->info("init logger : logFileName     = {}", logFileName);
        gLogger->info("init logger : minutes         = {}", minutes);
        gLogger->info("init logger : maxFileCount    = {}", maxFileCount);
    }
}

void set_log_level(int lvl)
{
    if (gLogger)
    {
        gLogger->set_level(static_cast<spdlog::level::level_enum>(lvl));
    }
}

void set_log_file_rotate_callback(log_file_rotate_callback_t cb)
{
    my_file_event_handlers_.cb_ = cb;
}

int do_log(int priority, char const * const fmt, va_list args)
{
    static char message[LOG_MAX_LENGTH] = {'\0'};
    memset(message, '\0', LOG_MAX_LENGTH);
    vsnprintf(message, LOG_MAX_LENGTH, fmt, args);

    switch (priority) {
        case _VERBOSE_:
            gLogger->trace(message);
            break;
        case _DEBUG_:
            gLogger->debug(message);
            break;
        case _INFO_:
            gLogger->info(message);
            break;
        case _WARN_:
            gLogger->warn(message);
            break;
        case _ERROR_:
            gLogger->error(message);
            break;
        case _FATAL_:
            gLogger->critical(message);
            break;
        default:
            gLogger->critical(message);
            break;
    }
    return 0;
}

int log(int priority, char const * fmt, ...)
{
    if (gLogger) {
        va_list args;
        va_start(args, fmt);
        int ret = do_log(priority, fmt, args);
        va_end(args);
        return ret;
    }
    return -1;
}

int log(int priority, char const * const fmt, va_list args)
{
    if (gLogger) {
        va_list args_cp;
	    va_copy(args_cp, args);
        int ret = do_log(priority, fmt, args);
        va_end(args_cp);
        return ret;
    }
    return -1;
}

}

