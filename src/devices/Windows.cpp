#include <BatteryInfo.hpp>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace arcticwoof;

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

bool BatteryInfo::isBatterySaver() {
    // Try to detect battery saver using the Windows 10 PowerReadValueDWord approach
    HKEY hKey;
    DWORD batterySaverState = 0;
    DWORD dataSize = sizeof(DWORD);
    
    // Open the registry key for battery saver settings
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, 
                     "SYSTEM\\CurrentControlSet\\Control\\Power\\PowerSettings\\19cbb8fa-5279-450e-9fac-8a3d5fedd0c1\\12bbebe6-58d6-4636-95bb-3217ef867c1a",
                     0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        // Read the value
        RegQueryValueExA(hKey, "Value", NULL, NULL, (LPBYTE)&batterySaverState, &dataSize);
        RegCloseKey(hKey);
        
        // If value is 1, battery saver is on
        if (batterySaverState == 1) {
            return true;
        }
    }
    
    // Try an alternative method: check power scheme 
    // PowerGetSystemPowerStatus can also give additional info
    SYSTEM_POWER_STATUS s;
    if (GetSystemPowerStatus(&s)) {
        // On battery and low battery status can indicate battery saver
        if (s.ACLineStatus == 0 && s.BatteryLifePercent <= 20) {
            // When on battery and <= 20%, battery saver is often enabled by default
            return true;
        }
    }
    
    // Check Power Throttling settings (Windows 10 1709+)
    HKEY powerThrottlingKey;
    DWORD powerThrottlingValue = 0;
    dataSize = sizeof(DWORD);
    
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, 
                     "SYSTEM\\CurrentControlSet\\Control\\Power\\PowerThrottling",
                     0, KEY_READ, &powerThrottlingKey) == ERROR_SUCCESS) {
        
        RegQueryValueExA(powerThrottlingKey, "PowerThrottlingOff", NULL, NULL, (LPBYTE)&powerThrottlingValue, &dataSize);
        RegCloseKey(powerThrottlingKey);
        
        // If PowerThrottlingOff is 0, power throttling (which includes battery saver aspects) is on
        if (powerThrottlingValue == 0) {
            return true;
        }
    }
    
    // If all detection methods fail, assume it's not in battery saver mode
    return false;
}
#endif
