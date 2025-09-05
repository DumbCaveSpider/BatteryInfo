#include <BatteryInfo.hpp>
// Check specifically for iOS
#if defined(__APPLE__) && (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)

using namespace arcticwoof;
    #import <UIKit/UIKit.h>

    float BatteryInfo::getBatteryLevel() {
        @autoreleasepool {
            UIDevice* d = [UIDevice currentDevice];
            d.batteryMonitoringEnabled = YES;
            float level = d.batteryLevel; // -1.0 if unknown
            if (level < 0.0f) return -1.0f;
            return level * 100.0f;
        }
    }

    bool BatteryInfo::isCharging() {
        @autoreleasepool {
            UIDevice* d = [UIDevice currentDevice];
            d.batteryMonitoringEnabled = YES;
            UIDeviceBatteryState s = d.batteryState;
            return (s == UIDeviceBatteryStateCharging || s == UIDeviceBatteryStateFull);
        }
    }

    bool BatteryInfo::isBatterySaverEnabled() {
        @autoreleasepool {
            // iOS 9.0+ supports Low Power Mode
            if (@available(iOS 9.0, *)) {
                NSProcessInfo* processInfo = [NSProcessInfo processInfo];
                return [processInfo isLowPowerModeEnabled];
            }
            return false;
        }
    }
#endif
