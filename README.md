# Battery Info API

A Geode Mod API for accessing battery information on various platforms (Windows, macOS, iOS, and Android).

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

### Methods

```cpp
// Get an instance of the API
BatteryInfo::get();

// Get battery level (-1 if unknown or error)
float getBatteryLevel();

// Check if device is charging
bool isCharging();

// Check if battery saver mode is enabled
bool isBatterySaver();

```

### Example

```cpp
#include <Geode/Geode.hpp>
#include <BatteryInfo.hpp>

using namespace geode::prelude;

class $modify(MyClass, SomeClass) {
    void someMethod() {
        auto batteryAPI = BatteryInfo::get();
        float level = batteryAPI->getBatteryLevel();
        bool charging = batteryAPI->isCharging();
        bool batterySaver = batteryAPI->isBatterySaverEnabled();

        log::info("Battery level: {}%, Charging: {}, Battery Saver: {}", level, charging ? "Yes" : "No", batterySaver ? "On" : "Off");
    }
};
```
*In this example, it logs the battery level, charging status, and battery saver mode status. But do keep in mind that this only fetches the battery status that executed this, so if you want to dynamically check for any changes on the battery status, you need to create a method that checks the battery status periodically.*