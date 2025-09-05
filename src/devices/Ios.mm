#include "../BatteryInfo.hpp"
#if defined(__APPLE__)
    // Check specifically for iOS
    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
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
    #else
        // macOS implementation - placeholder that returns default values
        float BatteryInfo::getBatteryLevel() {
            // Return -1.0f to indicate we don't have the information
            return -1.0f;
        }

        bool BatteryInfo::isCharging() {
            // Default to false on macOS (could implement real macOS battery detection later)
            return false;
        }
    #endif
#endif
