//
// Created by 刘新建 on 2023/3/9.
//

#ifndef ANDROID_LOGGER_ATTACHED_ENV_H
#define ANDROID_LOGGER_ATTACHED_ENV_H

#include <jni.h>
#include <android/log.h>

namespace logger {
    extern JavaVM *gJavaVM;
}

class attached_env final {
public:
    attached_env() {
        auto resCode = get_env(&mEnv);
        if (resCode == 2) {
            throw std::runtime_error("Cannot retrieve JNI environment");
        }
        needDetach = (resCode == 1);
    }

    ~attached_env() {
        if (needDetach) {
            logger::gJavaVM->DetachCurrentThread();
        }
    }

    JNIEnv *env() const noexcept {
        return mEnv;
    }

private:
    static int get_env(JNIEnv **g_env) {
        int getEnvStat = logger::gJavaVM->GetEnv((void **) g_env, JNI_VERSION_1_6);
        if (getEnvStat == JNI_EDETACHED) {
            if (logger::gJavaVM->AttachCurrentThread(g_env, nullptr) != 0) {
                __android_log_print(ANDROID_LOG_ERROR, "GetEnvironmentRoutine", "FAILED ATTACH THREAD");
                return 2; //Failed to attach
            }
            return 1; //Attached. Need detach
        }
        return 0;//Already attached
    }
    JNIEnv *mEnv = nullptr;
    bool needDetach = false;
};

#endif //ANDROID_LOGGER_ATTACHED_ENV_H
