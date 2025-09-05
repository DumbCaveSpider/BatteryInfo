#include <BatteryInfo.hpp>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#if defined(__has_include)
    #if __has_include(<winrt/Windows.System.Power.h>)
        #include <winrt/base.h>
        #include <winrt/Windows.System.Power.h>
        #define HAVE_WINRT_POWER 1
    #endif
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
#ifdef HAVE_WINRT_POWER
    // Try WinRT PowerManager first (Windows 11/10 with C++/WinRT available)
    // Initialize WinRT apartment for this thread (no exceptions handled here per project guidelines)
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    using namespace winrt::Windows::System::Power;
    auto status = PowerManager::EnergySaverStatus();
    return (status == EnergySaverStatus::On);
#else
    // Fallback: use SYSTEM_POWER_STATUS.SystemStatusFlag (Windows 10)
    SYSTEM_POWER_STATUS s;
    if (!GetSystemPowerStatus(&s))
        return false;
    return (s.SystemStatusFlag & 1) != 0;
#endif
}
#endif
