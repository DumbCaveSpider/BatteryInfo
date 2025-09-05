#pragma once

class BatteryInfo {
public:
    // returns battery level in percent (0..100), or -1 if unknown/error
    static float getBatteryLevel();

    // returns true if charging/plugged, false otherwise (or on error)
    static bool isCharging();
};
