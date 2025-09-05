#include "../BatteryInfo.hpp"
#if defined(__ANDROID__)
#include <jni.h>
#include <string>

extern JavaVM* g_jvm; // project usually provides this; adapt if different

static JNIEnv* getJniEnv() {
    if (!g_jvm) return nullptr;
    JNIEnv* env = nullptr;
    if (g_jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        if (g_jvm->AttachCurrentThread(&env, nullptr) != JNI_OK) return nullptr;
    }
    return env;
}

float BatteryInfo::getBatteryLevel() {
    JNIEnv* env = getJniEnv();
    if (!env) return -1.0f;
    jclass cls = env->FindClass("org/cocos2dx/cpp/AppActivity"); // adjust to your package
    if (!cls) return -1.0f;
    jmethodID mid = env->GetStaticMethodID(cls, "getBatteryLevel", "()I");
    if (!mid) return -1.0f;
    jint val = env->CallStaticIntMethod(cls, mid);
    return static_cast<float>(val);
}

bool BatteryInfo::isCharging() {
    JNIEnv* env = getJniEnv();
    if (!env) return false;
    jclass cls = env->FindClass("org/cocos2dx/cpp/AppActivity");
    if (!cls) return false;
    jmethodID mid = env->GetStaticMethodID(cls, "isCharging", "()Z");
    if (!mid) return false;
    jboolean val = env->CallStaticBooleanMethod(cls, mid);
    return val == JNI_TRUE;
}
#endif
