#include <BatteryInfo.hpp>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace arcticwoof;

int BatteryInfo::getBatteryLevel()
{
    SYSTEM_POWER_STATUS s;
    if (!GetSystemPowerStatus(&s))
        return -1;
    if (s.BatteryLifePercent == 255)
        return -1;
    return s.BatteryLifePercent;
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
    // This won't work on Windows 11 energy saver mode
    // yes that whole wall thing is just a mess
    SYSTEM_POWER_STATUS s;
    if (GetSystemPowerStatus(&s) && (s.SystemStatusFlag & 1) != 0)
    {
        return true;
    }
    return false;
}
#endif