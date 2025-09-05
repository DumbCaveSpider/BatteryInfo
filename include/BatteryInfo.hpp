#pragma once

#include <Geode/Geode.hpp>

// Define API export macros
#ifdef ARCTICWOOF_BATTERYINFO_API_EXPORTING
    #ifdef GEODE_IS_WINDOWS
        #define ARCTICWOOF_BATTERYINFO_DLL __declspec(dllexport)
    #else
        #define ARCTICWOOF_BATTERYINFO_DLL __attribute__((visibility("default")))
    #endif
#else
    #ifdef GEODE_IS_WINDOWS
        #define ARCTICWOOF_BATTERYINFO_DLL __declspec(dllimport)
    #else
        #define ARCTICWOOF_BATTERYINFO_DLL
    #endif
#endif

namespace arcticwoof {
    class ARCTICWOOF_BATTERYINFO_DLL BatteryInfo {
    private:
        struct Impl;
        std::unique_ptr<Impl> impl;
        static BatteryInfo* s_instance;

        // Make constructor private to enforce singleton pattern
        BatteryInfo();

    public:
        ~BatteryInfo();
        
        // Singleton access
        static BatteryInfo* get();
        
        // Returns battery level in percent (0..100), or -1 if unknown/error
        float getBatteryLevel();

        // Returns true if charging/plugged, false otherwise (or on error)
        bool isCharging();
        
        // Returns true if the device is in battery saver/low power mode, false otherwise (or on error)
        bool isBatterySaverEnabled();
    };
}
