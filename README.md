# Battery Info API

A Geode Mod API for accessing battery information on various platforms (Windows, macOS, iOS, and Android).
For end-users, you can enable the Battery Status display in the mod settings.

## Usage

First, add this mod as a dependency in your `mod.json`:

```json
"dependencies": {
    "arcticwoof.battery_info_api": {
        "version": ">=1.0.0"
    }
}
```

Then, include the header in your code:

```cpp
#include <arcticwoof.battery_info_api/include/BatteryInfo.hpp>
```

## Methods

```cpp
BatteryInfo::get(); // Get an instance of the API
int getBatteryLevel(); // Get battery level (-1 if unknown or error)
bool isCharging(); // Check if device is charging
bool isBatterySaver(); // Check if device is in low power/battery saver mode
```

## Example

```cpp
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <arcticwoof.battery_info_api/include/BatteryInfo.hpp>

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel *level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        auto batteryAPI = BatteryInfo::get();
        int level = batteryAPI->getBatteryLevel();
        bool charging = batteryAPI->isCharging();
        bool batterySaver = batteryAPI->isBatterySaver();
        log::info("Battery level: {}%, Charging: {}, Battery Saver: {}", 
                  level, charging ? "Yes" : "No", batterySaver ? "Yes" : "No");
        return true;
    }
};
```
*In this example, it logs the battery level and charging status when user enters a level. But do keep in mind that this only fetches the battery status that called this function once, so if you want to dynamically check for any changes on the battery status, you need to create a method that checks the battery status periodically.*

## Disclaimer
This is my first API mod and only intensively tested Windows and iOS. Any android implemetation that works are welcome!
## Dev Notes
- Windows 10 Low Battery mode works but Windows 11 Energy Saver doesn't work due of how Windows changed it.
- Currently, Android isn't supported at all as of now due to lack of testing.