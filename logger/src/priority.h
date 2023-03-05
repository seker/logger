//
// Created by xinjian
//

#ifndef ANDROID_ARCH_PRIORITY_INL_H
#define ANDROID_ARCH_PRIORITY_INL_H

#include <spdlog/common.h>

#include "logger.h"

namespace logger {
    inline spdlog::level::level_enum level(int priority) {
        switch (priority) {
            case _VERBOSE_:
                return spdlog::level::trace;
            case _DEBUG_:
                return spdlog::level::debug;
            case _INFO_:
                return spdlog::level::info;
            case _WARN_:
                return spdlog::level::warn;
            case _ERROR_:
                return spdlog::level::err;
            case _FATAL_:
                return spdlog::level::critical;
            default:
                return spdlog::level::off;
        }
    }
}

#endif //ANDROID_ARCH_PRIORITY_INL_H
