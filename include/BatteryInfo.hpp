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

        /// Make constructor private to enforce singleton pattern
        BatteryInfo();

    public:
        ~BatteryInfo();

        /// Gets the BatteryInfo instance
        /// @param None
        /// @return Pointer to the BatteryInfo instance
        static BatteryInfo* get();
        
        /// Gets the device battery level, or -1 if unknown/error
        /// @param None
        /// @return Battery level (0-100) or -1 if unknown/error
        int getBatteryLevel();

        /// Checks if charging/plugged or on error
        /// @param None
        /// @return True if charging, false otherwise
        bool isCharging();

        /// Checks if device is in low power/battery saver mode
        /// @param None
        /// @return True if battery saver is active, false otherwise
        bool isBatterySaver();
    };
}
