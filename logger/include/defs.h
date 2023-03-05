//
// Created by xinjian
//

#ifndef ANDROID_ARCH_DEFS_H
#define ANDROID_ARCH_DEFS_H

#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#define P2S(p)              (p ? "!nullptr" : "nullptr")
#define B2S(b)              (b ? "true" : "false")

#endif //ANDROID_ARCH_DEFS_H
