#include <BatteryInfo.hpp>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace arcticwoof;

float BatteryInfo::getBatteryLevel()
{
    SYSTEM_POWER_STATUS s;
    if (!GetSystemPowerStatus(&s))
        return -1.0f;
    if (s.BatteryLifePercent == 255)
        return -1.0f;
    return static_cast<float>(s.BatteryLifePercent);
}

bool BatteryInfo::isCharging()
{
    SYSTEM_POWER_STATUS s;
    if (!GetSystemPowerStatus(&s))
        return false;
    return (s.ACLineStatus == 1);
}

bool BatteryInfo::isBatterySaver()
{
    // Method 1: Check Windows 10/11 Energy Saver settings via PowerGetSystemPowerStatus
    SYSTEM_POWER_STATUS s;
    if (GetSystemPowerStatus(&s))
    {
        // Check PowerSaveStatus flag (bit 7) in BatteryFlag field
        if ((s.BatteryFlag & 0x08) != 0)
        {
            return true;
        }
    }
    return false;
}
#endif
