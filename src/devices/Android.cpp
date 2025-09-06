#include <BatteryInfo.hpp>
#if defined(__ANDROID__)
#include <jni.h>
#include <string>
#include <Geode/cocos/platform/android/jni/JniHelper.h>

using namespace arcticwoof;

// This has to be done. Sorry if I have sinned
namespace {
    // Fetch the current Activity via Cocos2dxHelper.getActivity()
    jobject getActivityLocalRef(JNIEnv* env) {
        cocos2d::JniMethodInfo t;
        if (!cocos2d::JniHelper::getStaticMethodInfo(
                t,
                "org/cocos2dx/lib/Cocos2dxHelper",
                "getActivity",
                "()Landroid/app/Activity;")) {
            return nullptr;
        }
        // Prefer the env from methodinfo to ensure it's valid for this thread
        env = t.env;
        jobject activity = env->CallStaticObjectMethod(t.classID, t.methodID);
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
            env->DeleteLocalRef(t.classID);
            return nullptr;
        }
        env->DeleteLocalRef(t.classID);
        return activity; // local ref
    }
}

int BatteryInfo::getBatteryLevel() {
    // Acquire env via JniHelper static method call
    cocos2d::JniMethodInfo t;
    if (!cocos2d::JniHelper::getStaticMethodInfo(
            t,
            "org/cocos2dx/lib/Cocos2dxHelper",
            "getActivity",
            "()Landroid/app/Activity;")) {
        return -1;
    }
    JNIEnv* env = t.env;
    jobject activity = env->CallStaticObjectMethod(t.classID, t.methodID);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        env->DeleteLocalRef(t.classID);
        return -1;
    }
    env->DeleteLocalRef(t.classID);
    if (activity == nullptr) return -1;

    // Context.BATTERY_SERVICE
    jclass contextCls = env->FindClass("android/content/Context");
    if (contextCls == nullptr) { env->DeleteLocalRef(activity); return -1; }
    jfieldID fidBatteryService = env->GetStaticFieldID(contextCls, "BATTERY_SERVICE", "Ljava/lang/String;");
    if (fidBatteryService == nullptr) { env->DeleteLocalRef(contextCls); env->DeleteLocalRef(activity); return -1; }
    jobject batteryServiceStr = env->GetStaticObjectField(contextCls, fidBatteryService);
    if (batteryServiceStr == nullptr) { env->DeleteLocalRef(contextCls); env->DeleteLocalRef(activity); return -1; }

    // activity.getSystemService(Context.BATTERY_SERVICE)
    jclass activityCls = env->GetObjectClass(activity);
    if (activityCls == nullptr) { env->DeleteLocalRef(batteryServiceStr); env->DeleteLocalRef(contextCls); env->DeleteLocalRef(activity); return -1; }
    jmethodID midGetSystemService = env->GetMethodID(activityCls, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    if (midGetSystemService == nullptr) { env->DeleteLocalRef(activityCls); env->DeleteLocalRef(batteryServiceStr); env->DeleteLocalRef(contextCls); env->DeleteLocalRef(activity); return -1; }
    jobject batteryMgrObj = env->CallObjectMethod(activity, midGetSystemService, batteryServiceStr);
    if (env->ExceptionCheck() || batteryMgrObj == nullptr) {
        env->ExceptionClear();
        env->DeleteLocalRef(activityCls);
        env->DeleteLocalRef(batteryServiceStr);
        env->DeleteLocalRef(contextCls);
        env->DeleteLocalRef(activity);
        return -1;
    }

    // BatteryManager.getIntProperty(BATTERY_PROPERTY_CAPACITY)
    jclass batteryMgrCls = env->FindClass("android/os/BatteryManager");
    if (batteryMgrCls == nullptr) {
        env->DeleteLocalRef(batteryMgrObj);
        env->DeleteLocalRef(activityCls);
        env->DeleteLocalRef(batteryServiceStr);
        env->DeleteLocalRef(contextCls);
        env->DeleteLocalRef(activity);
        return -1;
    }
    jmethodID midGetIntProperty = env->GetMethodID(batteryMgrCls, "getIntProperty", "(I)I");
    jfieldID fidCapacity = env->GetStaticFieldID(batteryMgrCls, "BATTERY_PROPERTY_CAPACITY", "I");
    if (midGetIntProperty == nullptr || fidCapacity == nullptr) {
        env->DeleteLocalRef(batteryMgrCls);
        env->DeleteLocalRef(batteryMgrObj);
        env->DeleteLocalRef(activityCls);
        env->DeleteLocalRef(batteryServiceStr);
        env->DeleteLocalRef(contextCls);
        env->DeleteLocalRef(activity);
        return -1;
    }
    jint capacityProp = env->GetStaticIntField(batteryMgrCls, fidCapacity);
    jint level = env->CallIntMethod(batteryMgrObj, midGetIntProperty, capacityProp);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        level = -1;
    }

    env->DeleteLocalRef(batteryMgrCls);
    env->DeleteLocalRef(batteryMgrObj);
    env->DeleteLocalRef(activityCls);
    env->DeleteLocalRef(batteryServiceStr);
    env->DeleteLocalRef(contextCls);
    env->DeleteLocalRef(activity);
    return static_cast<int>(level);
}

bool BatteryInfo::isCharging() {
    // Acquire env via JniHelper static method call
    cocos2d::JniMethodInfo t;
    if (!cocos2d::JniHelper::getStaticMethodInfo(
            t,
            "org/cocos2dx/lib/Cocos2dxHelper",
            "getActivity",
            "()Landroid/app/Activity;")) {
        return false;
    }
    JNIEnv* env = t.env;
    jobject activity = env->CallStaticObjectMethod(t.classID, t.methodID);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        env->DeleteLocalRef(t.classID);
        return false;
    }
    env->DeleteLocalRef(t.classID);
    if (activity == nullptr) return false;

    // Build IntentFilter for ACTION_BATTERY_CHANGED
    jclass intentCls = env->FindClass("android/content/Intent");
    jclass intentFilterCls = env->FindClass("android/content/IntentFilter");
    if (intentCls == nullptr || intentFilterCls == nullptr) {
        if (intentCls) env->DeleteLocalRef(intentCls);
        if (intentFilterCls) env->DeleteLocalRef(intentFilterCls);
        env->DeleteLocalRef(activity);
        return false;
    }

    jfieldID fidActionBatteryChanged = env->GetStaticFieldID(intentCls, "ACTION_BATTERY_CHANGED", "Ljava/lang/String;");
    if (fidActionBatteryChanged == nullptr) {
        env->DeleteLocalRef(intentFilterCls);
        env->DeleteLocalRef(intentCls);
        env->DeleteLocalRef(activity);
        return false;
    }
    jobject actionBatteryChanged = env->GetStaticObjectField(intentCls, fidActionBatteryChanged);
    if (actionBatteryChanged == nullptr) {
        env->DeleteLocalRef(intentFilterCls);
        env->DeleteLocalRef(intentCls);
        env->DeleteLocalRef(activity);
        return false;
    }

    jmethodID intentFilterCtor = env->GetMethodID(intentFilterCls, "<init>", "(Ljava/lang/String;)V");
    if (intentFilterCtor == nullptr) {
        env->DeleteLocalRef(actionBatteryChanged);
        env->DeleteLocalRef(intentFilterCls);
        env->DeleteLocalRef(intentCls);
        env->DeleteLocalRef(activity);
        return false;
    }
    jobject filterObj = env->NewObject(intentFilterCls, intentFilterCtor, actionBatteryChanged);
    if (env->ExceptionCheck() || filterObj == nullptr) {
        env->ExceptionClear();
        env->DeleteLocalRef(actionBatteryChanged);
        env->DeleteLocalRef(intentFilterCls);
        env->DeleteLocalRef(intentCls);
        env->DeleteLocalRef(activity);
        return false;
    }

    jclass activityCls = env->GetObjectClass(activity);
    jmethodID midRegisterReceiver = env->GetMethodID(activityCls, "registerReceiver", "(Landroid/content/BroadcastReceiver;Landroid/content/IntentFilter;)Landroid/content/Intent;");
    if (midRegisterReceiver == nullptr) {
        env->DeleteLocalRef(filterObj);
        env->DeleteLocalRef(activityCls);
        env->DeleteLocalRef(actionBatteryChanged);
        env->DeleteLocalRef(intentFilterCls);
        env->DeleteLocalRef(intentCls);
        env->DeleteLocalRef(activity);
        return false;
    }
    jobject batteryIntent = env->CallObjectMethod(activity, midRegisterReceiver, nullptr, filterObj);
    if (env->ExceptionCheck() || batteryIntent == nullptr) {
        env->ExceptionClear();
        env->DeleteLocalRef(filterObj);
        env->DeleteLocalRef(activityCls);
        env->DeleteLocalRef(actionBatteryChanged);
        env->DeleteLocalRef(intentFilterCls);
        env->DeleteLocalRef(intentCls);
        env->DeleteLocalRef(activity);
        return false;
    }

    // intent.getIntExtra(BatteryManager.EXTRA_STATUS, -1)
    jmethodID midGetIntExtra = env->GetMethodID(intentCls, "getIntExtra", "(Ljava/lang/String;I)I");
    jclass batteryMgrCls = env->FindClass("android/os/BatteryManager");
    if (midGetIntExtra == nullptr || batteryMgrCls == nullptr) {
        if (batteryMgrCls) env->DeleteLocalRef(batteryMgrCls);
        env->DeleteLocalRef(batteryIntent);
        env->DeleteLocalRef(filterObj);
        env->DeleteLocalRef(activityCls);
        env->DeleteLocalRef(actionBatteryChanged);
        env->DeleteLocalRef(intentFilterCls);
        env->DeleteLocalRef(intentCls);
        env->DeleteLocalRef(activity);
        return false;
    }
    jfieldID fidExtraStatus = env->GetStaticFieldID(batteryMgrCls, "EXTRA_STATUS", "Ljava/lang/String;");
    jfieldID fidStatusCharging = env->GetStaticFieldID(batteryMgrCls, "BATTERY_STATUS_CHARGING", "I");
    jfieldID fidStatusFull = env->GetStaticFieldID(batteryMgrCls, "BATTERY_STATUS_FULL", "I");
    if (fidExtraStatus == nullptr || fidStatusCharging == nullptr || fidStatusFull == nullptr) {
        env->DeleteLocalRef(batteryMgrCls);
        env->DeleteLocalRef(batteryIntent);
        env->DeleteLocalRef(filterObj);
        env->DeleteLocalRef(activityCls);
        env->DeleteLocalRef(actionBatteryChanged);
        env->DeleteLocalRef(intentFilterCls);
        env->DeleteLocalRef(intentCls);
        env->DeleteLocalRef(activity);
        return false;
    }
    jobject extraStatusKey = env->GetStaticObjectField(batteryMgrCls, fidExtraStatus);
    jint chargingConst = env->GetStaticIntField(batteryMgrCls, fidStatusCharging);
    jint fullConst = env->GetStaticIntField(batteryMgrCls, fidStatusFull);
    jint status = env->CallIntMethod(batteryIntent, midGetIntExtra, extraStatusKey, static_cast<jint>(-1));
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        status = -1;
    }

    bool isChg = (status == chargingConst) || (status == fullConst);

    // Cleanup
    env->DeleteLocalRef(extraStatusKey);
    env->DeleteLocalRef(batteryMgrCls);
    env->DeleteLocalRef(batteryIntent);
    env->DeleteLocalRef(filterObj);
    env->DeleteLocalRef(activityCls);
    env->DeleteLocalRef(actionBatteryChanged);
    env->DeleteLocalRef(intentFilterCls);
    env->DeleteLocalRef(intentCls);
    env->DeleteLocalRef(activity);
    return isChg;
}

bool BatteryInfo::isBatterySaver() {
    // Acquire env via JniHelper static method call
    cocos2d::JniMethodInfo t;
    if (!cocos2d::JniHelper::getStaticMethodInfo(
            t,
            "org/cocos2dx/lib/Cocos2dxHelper",
            "getActivity",
            "()Landroid/app/Activity;")) {
        return false;
    }
    JNIEnv* env = t.env;
    jobject activity = env->CallStaticObjectMethod(t.classID, t.methodID);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        env->DeleteLocalRef(t.classID);
        return false;
    }
    env->DeleteLocalRef(t.classID);
    if (activity == nullptr) return false;

    // Context.POWER_SERVICE
    jclass contextCls = env->FindClass("android/content/Context");
    if (contextCls == nullptr) { env->DeleteLocalRef(activity); return false; }
    jfieldID fidPowerService = env->GetStaticFieldID(contextCls, "POWER_SERVICE", "Ljava/lang/String;");
    if (fidPowerService == nullptr) { env->DeleteLocalRef(contextCls); env->DeleteLocalRef(activity); return false; }
    jobject powerServiceStr = env->GetStaticObjectField(contextCls, fidPowerService);
    if (powerServiceStr == nullptr) { env->DeleteLocalRef(contextCls); env->DeleteLocalRef(activity); return false; }

    // activity.getSystemService(Context.POWER_SERVICE)
    jclass activityCls = env->GetObjectClass(activity);
    if (activityCls == nullptr) { env->DeleteLocalRef(powerServiceStr); env->DeleteLocalRef(contextCls); env->DeleteLocalRef(activity); return false; }
    jmethodID midGetSystemService = env->GetMethodID(activityCls, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    if (midGetSystemService == nullptr) { env->DeleteLocalRef(activityCls); env->DeleteLocalRef(powerServiceStr); env->DeleteLocalRef(contextCls); env->DeleteLocalRef(activity); return false; }
    jobject powerMgrObj = env->CallObjectMethod(activity, midGetSystemService, powerServiceStr);
    if (env->ExceptionCheck() || powerMgrObj == nullptr) {
        env->ExceptionClear();
        env->DeleteLocalRef(activityCls);
        env->DeleteLocalRef(powerServiceStr);
        env->DeleteLocalRef(contextCls);
        env->DeleteLocalRef(activity);
        return false;
    }

    // PowerManager.isPowerSaveMode()
    jclass powerMgrCls = env->FindClass("android/os/PowerManager");
    if (powerMgrCls == nullptr) {
        env->DeleteLocalRef(powerMgrObj);
        env->DeleteLocalRef(activityCls);
        env->DeleteLocalRef(powerServiceStr);
        env->DeleteLocalRef(contextCls);
        env->DeleteLocalRef(activity);
        return false;
    }
    jmethodID midIsPSM = env->GetMethodID(powerMgrCls, "isPowerSaveMode", "()Z");
    if (midIsPSM == nullptr) {
        env->DeleteLocalRef(powerMgrCls);
        env->DeleteLocalRef(powerMgrObj);
        env->DeleteLocalRef(activityCls);
        env->DeleteLocalRef(powerServiceStr);
        env->DeleteLocalRef(contextCls);
        env->DeleteLocalRef(activity);
        return false;
    }
    jboolean val = env->CallBooleanMethod(powerMgrObj, midIsPSM);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        val = JNI_FALSE;
    }

    env->DeleteLocalRef(powerMgrCls);
    env->DeleteLocalRef(powerMgrObj);
    env->DeleteLocalRef(activityCls);
    env->DeleteLocalRef(powerServiceStr);
    env->DeleteLocalRef(contextCls);
    env->DeleteLocalRef(activity);
    return val == JNI_TRUE;
}

#endif
