//
// Created by xinjian
//

#include <jni.h>
#include <android/log.h>

#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include <spdlog/sinks/minutely_file_sink.h>

#include "logger.h"
#include "attached_env.h"

namespace logger {
    JavaVM *gJavaVM = nullptr;

    extern bool gConsole;
    extern int gPriority;
    extern std::shared_ptr<spdlog::sinks::minutely_file_sink_mt> minutelyFileSink;

    jobject gLogFileHandler= nullptr;
    jmethodID gMethodIDBeforeClose = nullptr;
    jmethodID gMethodIDAfterClose = nullptr;
    jmethodID gMethodIDBeforeOpen = nullptr;
    jmethodID gMethodIDAfterOpen = nullptr;

    static const char * const CLASS_NAME_LOGGER = "seker/logger/LogJni";

    void before_close(const std::string &filename) {
        attached_env a_env;
        JNIEnv *env = a_env.env();
        jstring fn = env->NewStringUTF(filename.c_str());
        env->CallVoidMethod(gLogFileHandler, gMethodIDBeforeClose, fn);
        env->DeleteLocalRef(fn);
    }

    void after_close(const std::string &filename) {
        attached_env a_env;
        JNIEnv *env = a_env.env();
        jstring fn = env->NewStringUTF(filename.c_str());
        env->CallVoidMethod(gLogFileHandler, gMethodIDAfterClose, fn);
        env->DeleteLocalRef(fn);
    }

    void before_open(const std::string &filename) {
        attached_env a_env;
        JNIEnv *env = a_env.env();
        jstring fn = env->NewStringUTF(filename.c_str());
        env->CallVoidMethod(gLogFileHandler, gMethodIDBeforeOpen, fn);
        env->DeleteLocalRef(fn);
    }

    void after_open(const std::string &filename) {
        attached_env a_env;
        JNIEnv *env = a_env.env();
        jstring fn = env->NewStringUTF(filename.c_str());
        env->CallVoidMethod(gLogFileHandler, gMethodIDAfterOpen, fn);
        env->DeleteLocalRef(fn);
    }
}

using namespace logger;

jint native_init(JNIEnv *env, jobject thiz, jstring rootDir, jstring baseLogFileName, jobject logFileHandler)
{
    const char *_rootDir = env->GetStringUTFChars(rootDir, nullptr);
    const char *_baseLogFileName = env->GetStringUTFChars(baseLogFileName, nullptr);

    gLogFileHandler = env->NewGlobalRef(logFileHandler);
    jclass classLogFileCallback = env->GetObjectClass(gLogFileHandler);
    gMethodIDBeforeOpen = env->GetMethodID(classLogFileCallback, "beforeOpen", "(Ljava/lang/String;)V");
    gMethodIDAfterOpen = env->GetMethodID(classLogFileCallback, "afterOpen", "(Ljava/lang/String;)V");
    gMethodIDBeforeClose = env->GetMethodID(classLogFileCallback, "beforeClose", "(Ljava/lang/String;)V");
    gMethodIDAfterClose = env->GetMethodID(classLogFileCallback, "afterClose", "(Ljava/lang/String;)V");
    env->DeleteLocalRef(classLogFileCallback);

    jint ret = init(_rootDir, _baseLogFileName, gConsole, gPriority, 20, before_open, after_open, before_close, after_close);

    env->ReleaseStringUTFChars(baseLogFileName, _baseLogFileName);
    env->ReleaseStringUTFChars(rootDir, _rootDir);

    return ret;
}

jint native_set_console(JNIEnv *env, jobject thiz, jboolean console)
{
    LOGD("Jni", "setConsole: %s", B2S(console));
    return setConsole(console);
}

jint native_set_priority(JNIEnv *env, jobject thiz, jint priority)
{
    LOGD("Jni", "setPriority: %d",priority);
    return setPriority(priority);
}

jint native_set_minutes(JNIEnv *env, jobject thiz, jint minutes)
{
    LOGD("Jni", "setMinutes: %d",minutes);
    return setMinutes(minutes);
}

jint native_log(JNIEnv *env, jobject thiz, jint priority, jstring tag, jstring threadName, jstring msg)
{
    if (gPriority > priority)
    {
        return -1;
    }

    const char *_tag = env->GetStringUTFChars(tag, nullptr);
    const char *_threadName = env->GetStringUTFChars(threadName, nullptr);
    const char *_msg = env->GetStringUTFChars(msg, nullptr);

    jint ret = log(priority, _tag, _threadName, _msg);

    env->ReleaseStringUTFChars(msg, _msg);
    env->ReleaseStringUTFChars(threadName, _threadName);
    env->ReleaseStringUTFChars(tag, _tag);

    return ret;
}

jint native_flush(JNIEnv *env, jobject thiz)
{
    return flush();
}

JNINativeMethod jniMethods_Logger[] = {
        {"init",        "(Ljava/lang/String;Ljava/lang/String;Lseker/logger/LogFileHandler;)I",     (void *) &native_init},
        {"setConsole",  "(Z)I",                                                                     (void *) &native_set_console},
        {"setPriority", "(I)I",                                                                     (void *) &native_set_priority},
        {"setMinutes",  "(I)I",                                                                     (void *) &native_set_minutes},
        {"log",         "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)I",               (void *) &native_log},
        {"flush",       "()I",                                                                      (void *) &native_flush},
};

void registerLoggerNativeMethods(JNIEnv *env)
{
    jclass clz = env->FindClass(logger::CLASS_NAME_LOGGER);
    jint ret = env->RegisterNatives(clz, jniMethods_Logger, sizeof(jniMethods_Logger) / sizeof(JNINativeMethod));
    LOGD("Jni", "RegisterNatives() : ret=%d", ret);
    env->DeleteLocalRef(clz);
}

void unregisterLoggerNativeMethods(JNIEnv *env)
{
    jclass clz = env->FindClass(logger::CLASS_NAME_LOGGER);
    jint ret = env->UnregisterNatives(clz);
    LOGD("Jni", "UnregisterNatives() : ret=%d", ret);
    env->DeleteLocalRef(clz);
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv *env;

    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return -1;
    }

    gJavaVM = vm;

    LOGD("logger", "JNI_OnLoad() : gJavaVM=%p", gJavaVM);

    registerLoggerNativeMethods(env);

    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved)
{
    JNIEnv *env;

    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return;
    }

    unregisterLoggerNativeMethods(env);

    LOGD("logger", "JNI_OnUnload()");
}

