#include <BatteryInfo.hpp>
#if defined(__ANDROID__)
#include <jni.h>
#include <string>
#include <Geode/cocos/platform/android/jni/JniHelper.h>

using namespace arcticwoof;

namespace {
    constexpr const char* kProviderClass = "io/github/arcticwoof/batteryinfo/BatteryInfoProvider";

    inline bool getStaticMethod(
        const char* clsName,
        const char* methodName,
        const char* signature,
        cocos2d::JniMethodInfo& out
    ) {
        return cocos2d::JniHelper::getStaticMethodInfo(out, clsName, methodName, signature);
    }
}

int BatteryInfo::getBatteryLevel() {
    cocos2d::JniMethodInfo t;
    if (!getStaticMethod(kProviderClass, "getBatteryLevel", "()I", t)) {
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
    if (!getStaticMethod(kProviderClass, "isCharging", "()Z", t)) {
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
    if (!getStaticMethod(kProviderClass, "isPowerSaveMode", "()Z", t)) {
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
