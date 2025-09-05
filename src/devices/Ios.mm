// Check specifically for iOS
#if defined(__APPLE__) && (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)

// First include Apple system headers before any Geode headers
#import <UIKit/UIKit.h>

// Then include our headers after all Apple headers
#include <BatteryInfo.hpp>

using namespace arcticwoof;

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

#endif
