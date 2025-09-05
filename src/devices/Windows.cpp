#include <BatteryInfo.hpp>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef USE_WINRT_POWER
#if defined(__has_include)
#if __has_include(<winrt/Windows.System.Power.h>)
#include <winrt/base.h>
#include <winrt/Windows.System.Power.h>
#define HAVE_WINRT_POWER 1
#endif
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
    // Initialize WinRT apartment for this thread
    winrt::init_apartment(winrt::apartment_type::single_threaded);

    // Get current Energy Saver status from PowerManager
    using namespace winrt::Windows::System::Power;
    auto status = PowerManager::EnergySaverStatus();

    // Return true only if Energy Saver is actively enabled (On state)
    return (status == EnergySaverStatus::On);
#else
    // Use multiple detection methods for better reliability

    // Method 1: Check SystemStatusFlag (basic battery saver)
    SYSTEM_POWER_STATUS s;
    if (GetSystemPowerStatus(&s) && (s.SystemStatusFlag & 1) != 0)
    {
        return true;
    }

    // Method 2: Check power scheme settings via registry (works for both Battery Saver and Energy Saver)
    HKEY hKey;
    bool energySaverEnabled = false;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     "SYSTEM\\CurrentControlSet\\Control\\Power\\PowerSettings\\DE830923-A562-41AF-A086-E3A2C6BAD2DA\\75b0ae3f-bce0-45a7-8c89-c9611c25e100",
                     0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        DWORD value = 0;
        DWORD dataSize = sizeof(DWORD);
        DWORD dataType = REG_DWORD;

        if (RegQueryValueEx(hKey, "ACSettingIndex", nullptr, &dataType,
                            reinterpret_cast<LPBYTE>(&value), &dataSize) == ERROR_SUCCESS)
        {
            // Value 1 means Energy Saver is on for AC power
            if (value == 1)
                energySaverEnabled = true;
        }

        // Check DC (battery) setting if AC setting wasn't enabled
        if (!energySaverEnabled &&
            RegQueryValueEx(hKey, "DCSettingIndex", nullptr, &dataType,
                            reinterpret_cast<LPBYTE>(&value), &dataSize) == ERROR_SUCCESS)
        {
            // Value 1 means Energy Saver is on for battery power
            if (value == 1)
                energySaverEnabled = true;
        }

        RegCloseKey(hKey);
        if (energySaverEnabled)
            return true;
    }

    // Method 3: Check PowerMode registry value (Windows 11)
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     "SYSTEM\\CurrentControlSet\\Control\\Power\\User\\PowerSchemes\\PowerSettingIndex",
                     0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        DWORD value = 0;
        DWORD dataSize = sizeof(DWORD);
        DWORD dataType = REG_DWORD;

        if (RegQueryValueEx(hKey, "PowerMode", nullptr, &dataType,
                            reinterpret_cast<LPBYTE>(&value), &dataSize) == ERROR_SUCCESS)
        {
            // PowerMode = 1 indicates "Best energy efficiency" (Energy Saver)
            if (value == 1)
            {
                RegCloseKey(hKey);
                return true;
            }
        }
        RegCloseKey(hKey);
    }

    // All detection methods failed - not in battery/energy saver mode
    return false;
#endif
}
#endif
