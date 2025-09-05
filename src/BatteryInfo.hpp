#pragma once

class BatteryInfo {
public:
    // returns battery level in percent (0..100), or -1 if unknown/error
    static float getBatteryLevel();

    // returns true if charging/plugged, false otherwise (or on error)
    static bool isCharging();
    
    // Returns a battery level that can be used for animations (0.0f to 1.0f)
    static inline float getNormalizedBatteryLevel() {
        float level = getBatteryLevel();
        return (level >= 0.0f) ? (level / 100.0f) : 0.0f;
    }
    
    // Returns if the battery is low (below 20%)
    static inline bool isBatteryLow() {
        float level = getBatteryLevel();
        return (level >= 0.0f && level <= 20.0f);
    }
};
