# Battery Info API

An API for accessing battery information on various platforms (Windows, macOS, iOS, and Android).

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

### Available Methods

```cpp
// Get an instance of the API
BatteryInfo::get();

// Get battery level (0-100%, returns -1 if unknown/error)
float getBatteryLevel();

// Check if device is charging
bool isCharging();

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

## Supported Platforms

- Windows
- macOS
- iOS
- Android

## Notice
<cy>- You are free to modify or use any of the code using in this repository. If you want to implement your own code, feel free to do so.</cy>