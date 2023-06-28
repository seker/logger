//
// Created by 刘新建 on 2023/3/9.
//

#ifndef ANDROID_LOGGER_ATTACHED_ENV_H
#define ANDROID_LOGGER_ATTACHED_ENV_H

#include <jni.h>

class attached_env final {
public:
    attached_env();
    ~attached_env();
    JNIEnv* env() const noexcept;

private:
    static int get_env(JNIEnv **g_env);
    JNIEnv *mEnv = nullptr;
    bool needDetach = false;
};

#endif //ANDROID_LOGGER_ATTACHED_ENV_H
