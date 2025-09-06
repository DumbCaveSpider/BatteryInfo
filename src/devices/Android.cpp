#include <BatteryInfo.hpp>
#if defined(__ANDROID__)
#include <jni.h>
#include <string>
#include <Geode/cocos/platform/android/jni/JniHelper.h>

using namespace arcticwoof;

// This has to be done. Sorry if I have sinned
namespace {
    // Java provider class path
    constexpr const char* kProviderClass = "com/arcticwoof/batteryinfo/BatteryInfoProvider";
}

int BatteryInfo::getBatteryLevel() {
    cocos2d::JniMethodInfo t;
    if (!cocos2d::JniHelper::getStaticMethodInfo(t, kProviderClass, "getBatteryLevel", "()I")) {
        return -1;
    }
    jint val = t.env->CallStaticIntMethod(t.classID, t.methodID);
    if (t.env->ExceptionCheck()) {
        t.env->ExceptionClear();
        t.env->DeleteLocalRef(t.classID);
        return -1;
    }
    t.env->DeleteLocalRef(t.classID);
    return static_cast<int>(val);
}

bool BatteryInfo::isCharging() {
    cocos2d::JniMethodInfo t;
    if (!cocos2d::JniHelper::getStaticMethodInfo(t, kProviderClass, "isCharging", "()Z")) {
        return false;
    }
    jboolean val = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
    if (t.env->ExceptionCheck()) {
        t.env->ExceptionClear();
        t.env->DeleteLocalRef(t.classID);
        return false;
    }
    t.env->DeleteLocalRef(t.classID);
    return val == JNI_TRUE;
}

bool BatteryInfo::isBatterySaver() {
    cocos2d::JniMethodInfo t;
    if (!cocos2d::JniHelper::getStaticMethodInfo(t, kProviderClass, "isPowerSaveMode", "()Z")) {
        return false;
    }
    jboolean val = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
    if (t.env->ExceptionCheck()) {
        t.env->ExceptionClear();
        t.env->DeleteLocalRef(t.classID);
        return false;
    }
    t.env->DeleteLocalRef(t.classID);
    return val == JNI_TRUE;
}

#endif
