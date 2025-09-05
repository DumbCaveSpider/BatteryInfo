#include <BatteryInfo.hpp>
#if defined(__ANDROID__)
#include <jni.h>
#include <string>
#include <Geode/cocos/platform/android/jni/JniHelper.h>

using namespace arcticwoof;

static JNIEnv* getJniEnv() {
    auto jvm = cocos2d::JniHelper::getJavaVM();
    if (!jvm) return nullptr;
    JNIEnv* env = nullptr;
    if (jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        if (jvm->AttachCurrentThread(&env, nullptr) != JNI_OK) return nullptr;
    }
    return env;
}

float BatteryInfo::getBatteryLevel() {
    JNIEnv* env = getJniEnv();
    if (!env) return -1.0f;
    
    // Find the BatteryInfoProvider class
    jclass cls = env->FindClass("io/github/arcticwoof/batteryinfo/BatteryInfoProvider");
    if (!cls) {
        // Class not found
        return -1.0f;
    }
    
    // Find the battery level method
    jmethodID mid = env->GetStaticMethodID(cls, "getBatteryLevel", "()I");
    if (!mid) {
        // Method not found
        env->DeleteLocalRef(cls);
        return -1.0f;
    }
    
    jint val = env->CallStaticIntMethod(cls, mid);
    env->DeleteLocalRef(cls);
    return static_cast<float>(val);
}

bool BatteryInfo::isCharging() {
    JNIEnv* env = getJniEnv();
    if (!env) return false;
    
    // Find the BatteryInfoProvider class
    jclass cls = env->FindClass("io/github/arcticwoof/batteryinfo/BatteryInfoProvider");
    if (!cls) {
        // Class not found
        return false;
    }
    
    // Find the charging state method
    jmethodID mid = env->GetStaticMethodID(cls, "isCharging", "()Z");
    if (!mid) {
        // Method not found
        env->DeleteLocalRef(cls);
        return false;
    }
    
    jboolean val = env->CallStaticBooleanMethod(cls, mid);
    env->DeleteLocalRef(cls);
    return val == JNI_TRUE;
}

#endif
