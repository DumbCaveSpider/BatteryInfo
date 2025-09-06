#if defined(__APPLE__) && (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
#import <UIKit/UIKit.h>
#include <BatteryInfo.hpp>

using namespace arcticwoof;

    int BatteryInfo::getBatteryLevel() {
        @autoreleasepool {
            UIDevice* d = [UIDevice currentDevice];
            if (!d.batteryMonitoringEnabled) {
                d.batteryMonitoringEnabled = YES;
                [NSThread sleepForTimeInterval:0.05];
            }
            
            float level = d.batteryLevel; // -1.0 if unknown
            if (level < 0.0f) return -1;
            return static_cast<int>(level * 100.0f);
        }
    }

    bool BatteryInfo::isCharging() {
        @autoreleasepool {
            UIDevice* d = [UIDevice currentDevice];
            if (!d.batteryMonitoringEnabled) {
                d.batteryMonitoringEnabled = YES;
                [NSThread sleepForTimeInterval:0.05];
            }
            
            UIDeviceBatteryState s = d.batteryState;
            return (s == UIDeviceBatteryStateCharging || s == UIDeviceBatteryStateFull);
        }
    }
    
    bool BatteryInfo::isBatterySaver() {
        @autoreleasepool {
            NSProcessInfo* processInfo = [NSProcessInfo processInfo];
            return [processInfo isLowPowerModeEnabled];
        }
    }

#endif
