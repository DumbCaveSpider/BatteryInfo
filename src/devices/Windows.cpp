#include <BatteryInfo.hpp>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#if __has_include(<winrt/Windows.System.Power.h>)
// Prefer WinRT EnergySaverStatus on Windows 10/11+
#include <winrt/base.h>
#include <winrt/Windows.System.Power.h>
#pragma comment(lib, "WindowsApp.lib")
#endif

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
    // I hate this
    // also this part is the part I sinned, the rest I actually didnt so stop saying its 100%
#if __has_include(<winrt/Windows.System.Power.h>)
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    auto status = winrt::Windows::System::Power::PowerManager::EnergySaverStatus();
    return status == winrt::Windows::System::Power::EnergySaverStatus::On;
#else
    SYSTEM_POWER_STATUS s;
    if (GetSystemPowerStatus(&s)) {
        return (s.SystemStatusFlag & 1) != 0;
    }
    return false;
#endif
}
#endif