#include <BatteryInfo.hpp>
// Check specifically for iOS
#if defined(__APPLE__) && (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)

// Fix for CommentType name conflict between macOS SDK and Geode
#pragma push_macro("CommentType")
#undef CommentType

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

// Restore CommentType macro
#pragma pop_macro("CommentType")

#endif
