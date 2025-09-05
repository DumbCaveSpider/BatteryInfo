#include "../BatteryInfo.hpp"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

float BatteryInfo::getBatteryLevel() {
    SYSTEM_POWER_STATUS s;
    if (!GetSystemPowerStatus(&s)) return -1.0f;
    if (s.BatteryLifePercent == 255) return -1.0f;
    return static_cast<float>(s.BatteryLifePercent);
}

bool BatteryInfo::isCharging() {
    SYSTEM_POWER_STATUS s;
    if (!GetSystemPowerStatus(&s)) return false;
    return (s.ACLineStatus == 1);
}
#endif
