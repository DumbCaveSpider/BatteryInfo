#if defined(__APPLE__) && !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
#import <Foundation/Foundation.h>
#import <IOKit/ps/IOPowerSources.h>
#import <IOKit/ps/IOPSKeys.h>
#include <BatteryInfo.hpp>

using namespace arcticwoof;

float BatteryInfo::getBatteryLevel() {
    @autoreleasepool {
        CFTypeRef powerSourceInfo = IOPSCopyPowerSourcesInfo();
        if (!powerSourceInfo) return -1.0f;
        
        CFArrayRef powerSources = IOPSCopyPowerSourcesList(powerSourceInfo);
        if (!powerSources) {
            CFRelease(powerSourceInfo);
            return -1.0f;
        }
        
        CFIndex count = CFArrayGetCount(powerSources);
        if (count == 0) {
            CFRelease(powerSources);
            CFRelease(powerSourceInfo);
            return -1.0f;
        }
        
        CFDictionaryRef powerSource = NULL;
        for (CFIndex i = 0; i < count; i++) {
            powerSource = (CFDictionaryRef)CFArrayGetValueAtIndex(powerSources, i);
            CFStringRef powerSourceState = (CFStringRef)CFDictionaryGetValue(powerSource, CFSTR(kIOPSTypeKey));
            
            if (powerSourceState && CFEqual(powerSourceState, CFSTR(kIOPSInternalBatteryType))) {
                break;
            }
            powerSource = NULL;
        }
        
        if (!powerSource) {
            CFRelease(powerSources);
            CFRelease(powerSourceInfo);
            return -1.0f;
        }
        
        CFNumberRef currentCapacity = (CFNumberRef)CFDictionaryGetValue(powerSource, CFSTR(kIOPSCurrentCapacityKey));
        CFNumberRef maxCapacity = (CFNumberRef)CFDictionaryGetValue(powerSource, CFSTR(kIOPSMaxCapacityKey));
        
        int currentValue = 0;
        int maxValue = 0;
        
        if (currentCapacity && maxCapacity &&
            CFNumberGetValue(currentCapacity, kCFNumberIntType, &currentValue) &&
            CFNumberGetValue(maxCapacity, kCFNumberIntType, &maxValue) &&
            maxValue > 0) {
            
            float percentage = (float)currentValue / (float)maxValue * 100.0f;
            
            CFRelease(powerSources);
            CFRelease(powerSourceInfo);
            
            return percentage;
        }
        
        CFRelease(powerSources);
        CFRelease(powerSourceInfo);
    }
    
    return -1.0f;
}

bool BatteryInfo::isCharging() {
    @autoreleasepool {
        CFTypeRef powerSourceInfo = IOPSCopyPowerSourcesInfo();
        if (!powerSourceInfo) return false;
        
        CFArrayRef powerSources = IOPSCopyPowerSourcesList(powerSourceInfo);
        if (!powerSources) {
            CFRelease(powerSourceInfo);
            return false;
        }
        
        CFIndex count = CFArrayGetCount(powerSources);
        if (count == 0) {
            CFRelease(powerSources);
            CFRelease(powerSourceInfo);
            return false;
        }
        
        CFDictionaryRef powerSource = NULL;
        for (CFIndex i = 0; i < count; i++) {
            powerSource = (CFDictionaryRef)CFArrayGetValueAtIndex(powerSources, i);
            CFStringRef powerSourceState = (CFStringRef)CFDictionaryGetValue(powerSource, CFSTR(kIOPSTypeKey));
            
            if (powerSourceState && CFEqual(powerSourceState, CFSTR(kIOPSInternalBatteryType))) {
                break;
            }
            powerSource = NULL;
        }
        
        if (!powerSource) {
            CFRelease(powerSources);
            CFRelease(powerSourceInfo);
            return false;
        }
        
        CFStringRef charging = (CFStringRef)CFDictionaryGetValue(powerSource, CFSTR(kIOPSPowerSourceStateKey));
        
        bool isCharging = charging && CFEqual(charging, CFSTR(kIOPSACPowerValue));
        
        CFRelease(powerSources);
        CFRelease(powerSourceInfo);
        
        return isCharging;
    }
}

bool BatteryInfo::isBatterySaver() {
    @autoreleasepool {
        // macOS 10.14+ supports Low Power Mode
        if (@available(macOS 10.14, *)) {
            // Check if system is in Low Power Mode
            // Note: Prior to macOS 12, there was no explicit "Low Power Mode"
            // but there was "automatic graphics switching" which is similar
            NSProcessInfo* processInfo = [NSProcessInfo processInfo];
            
            // Starting with macOS 12 (Monterey), Low Power Mode is officially supported
            if (@available(macOS 12.0, *)) {
                return processInfo.lowPowerModeEnabled;
            }
            
            // For older macOS versions, we can check energy saving settings
            // This is an approximation as older macOS doesn't have a true "Low Power Mode"
            return false;
        }
        return false;
    }
}

#endif
