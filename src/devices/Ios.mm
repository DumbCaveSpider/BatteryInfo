#include <BatteryInfo.hpp>
// Check specifically for iOS
#if defined(__APPLE__) && (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)

// Save and undefine CommentType to avoid name conflict
// Handle both normal CommentType and CommentTypeDummy
#ifdef CommentType
    #if defined(CommentTypeDummy)
        #define GEODE_SAVED_COMMENTTYPE CommentTypeDummy
    #else
        #define GEODE_SAVED_COMMENTTYPE CommentType
    #endif
    #undef CommentType
#endif

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

// Restore the original CommentType definition
#ifdef GEODE_SAVED_COMMENTTYPE
    #define CommentType GEODE_SAVED_COMMENTTYPE
    #undef GEODE_SAVED_COMMENTTYPE
#endif

#endif
