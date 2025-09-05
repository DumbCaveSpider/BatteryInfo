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
    // Windows 10 1709+ supports querying battery saver mode
    SYSTEM_POWER_STATUS s;
    if (!GetSystemPowerStatus(&s))
        return false;
    return (s.SystemStatusFlag & 1) != 0;
}
#endif
