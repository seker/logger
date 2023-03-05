//
// Created by xinjian
//

#include <jni.h>
#include <android/log.h>

#include "logger.h"
#include "attached_env.h"

namespace logger {
    JavaVM *gJavaVM = nullptr;

    extern bool gConsole;
    extern int gPriority;

    jobject gFileHandler = nullptr;
    jmethodID gMethodIDBeforeClose = nullptr;
    jmethodID gMethodIDAfterClose = nullptr;
    jmethodID gMethodIDBeforeOpen = nullptr;
    jmethodID gMethodIDAfterOpen = nullptr;

    static const char *CLASS_NAME_LOGGER = "seker/logger/LogJni";

    void before_close(const std::string &filename) {
        attached_env a_env;
        JNIEnv *env = a_env.env();
        jstring fn = env->NewStringUTF(filename.c_str());
        env->CallVoidMethod(gFileHandler, gMethodIDBeforeClose, fn);
        env->DeleteLocalRef(fn);
    }

    void after_close(const std::string &filename) {
        attached_env a_env;
        JNIEnv *env = a_env.env();
        jstring fn = env->NewStringUTF(filename.c_str());
        env->CallVoidMethod(gFileHandler, gMethodIDAfterClose, fn);
        env->DeleteLocalRef(fn);
    }

    void before_open(const std::string &filename) {
        attached_env a_env;
        JNIEnv *env = a_env.env();
        jstring fn = env->NewStringUTF(filename.c_str());
        env->CallVoidMethod(gFileHandler, gMethodIDBeforeOpen, fn);
        env->DeleteLocalRef(fn);
    }

    void after_open(const std::string &filename) {
        attached_env a_env;
        JNIEnv *env = a_env.env();
        jstring fn = env->NewStringUTF(filename.c_str());
        env->CallVoidMethod(gFileHandler, gMethodIDAfterOpen, fn);
        env->DeleteLocalRef(fn);
    }
}

using namespace logger;

void jni_initialize(JNIEnv *env, jclass clazz, jobject initConfig) {
    jclass classInitConfig = env->GetObjectClass(initConfig);

    jfieldID id_console = env->GetFieldID(classInitConfig, "console", "Z");
    gConsole = env->GetBooleanField(initConfig, id_console);

    jfieldID id_priority = env->GetFieldID(classInitConfig, "priority", "I");
    gPriority = env->GetIntField(initConfig, id_priority);

    jfieldID id_root_dir = env->GetFieldID(classInitConfig, "rootDir", "Ljava/lang/String;");
    auto _rooDir = (jstring) env->GetObjectField(initConfig, id_root_dir);
    const char *rootDir = env->GetStringUTFChars(_rooDir, nullptr);

    jfieldID id_process_name = env->GetFieldID(classInitConfig, "processName", "Ljava/lang/String;");
    auto _processName = (jstring) env->GetObjectField(initConfig, id_process_name);
    const char *processName = env->GetStringUTFChars(_processName, nullptr);

    jfieldID id_max_file_count = env->GetFieldID(classInitConfig, "maxFileCount", "I");
    int maxFileCount = env->GetIntField(initConfig, id_max_file_count);

    jfieldID id_minutes = env->GetFieldID(classInitConfig, "minutes", "I");
    int minutes = env->GetIntField(initConfig, id_minutes);

    jfieldID id_file_handler = env->GetFieldID(classInitConfig, "fileHandler",
                                               "Lcom/momenta/android/logger/impl/LogFileHandler;");
    jobject fileHandler = env->GetObjectField(initConfig, id_file_handler);
    gFileHandler = env->NewGlobalRef(fileHandler);

    jclass classLogFileHandler = env->GetObjectClass(gFileHandler);
    gMethodIDBeforeOpen = env->GetMethodID(classLogFileHandler, "beforeOpen", "(Ljava/lang/String;)V");
    gMethodIDAfterOpen = env->GetMethodID(classLogFileHandler, "afterOpen", "(Ljava/lang/String;)V");
    gMethodIDBeforeClose = env->GetMethodID(classLogFileHandler, "beforeClose", "(Ljava/lang/String;)V");
    gMethodIDAfterClose = env->GetMethodID(classLogFileHandler, "afterClose", "(Ljava/lang/String;)V");

    init(rootDir, processName);

    env->DeleteLocalRef(classLogFileHandler);
    env->DeleteLocalRef(fileHandler);
    env->ReleaseStringUTFChars(_processName, processName);
    env->ReleaseStringUTFChars(_rooDir, rootDir);
    env->DeleteLocalRef(classInitConfig);
}

void jni_set_priority(JNIEnv *env, jclass clazz, jint priority) {
    gPriority = priority;
}

jint jni_log(JNIEnv *env, jclass clazz, jint priority, jstring tag, jstring threadName, jstring msg) {
    if (gPriority > priority) {
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

void jni_flush(JNIEnv *env, jclass clazz) {
    flush();
}

JNINativeMethod jniMethods_Logger[] = {
//        {"initialize",              "(Lcom/momenta/android/logger/impl/InitConfig;)V",              (void *) &jni_initialize},
//        {"priority",                "(I)V",                                                         (void *) &jni_set_priority},
//        {"log",                     "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)I",   (void *) &jni_log},
//        {"flushToFile",             "()V",                                                          (void *) &jni_flush},
};

void registerLoggerNativeMethods(JNIEnv *env) {
    jclass clz = env->FindClass(logger::CLASS_NAME_LOGGER);
    env->RegisterNatives(clz, jniMethods_Logger, sizeof(jniMethods_Logger) / sizeof(JNINativeMethod));
    env->DeleteLocalRef(clz);
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;

    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    gJavaVM = vm;

    LOGD("logger", "JNI_OnLoad() : gJavaVM=%p", gJavaVM);

    registerLoggerNativeMethods(env);

    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    LOGD("logger", "JNI_OnUnload()");
}

