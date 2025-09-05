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

## Methods

```cpp
BatteryInfo::get(); // Get an instance of the API
float getBatteryLevel(); // Get battery level (-1 if unknown or error)
bool isCharging(); // Check if device is charging

```

## Example

```cpp
#include <Geode/Geode.hpp>
#include <arcticwoof.battery_info_api/include/BatteryInfo.hpp>

using namespace geode::prelude;

class $modify(MyClass, SomeClass) {
    void someMethod() {
        auto batteryAPI = BatteryInfo::get();
        float level = batteryAPI->getBatteryLevel();
        bool charging = batteryAPI->isCharging();
        log::info("Battery level: {}%, Charging: {}", level, charging ? "Yes" : "No");
    }
};
```
*In this example, it logs the battery level and charging status. But do keep in mind that this only fetches the battery status that executed this, so if you want to dynamically check for any changes on the battery status, you need to create a method that checks the battery status periodically.*

## Disclaimer
This is my first API mod and only intensively tested Windows and iOS only. So if you find any issues on Android or macOS, please report them so I can fix them. Pull requests are also welcome!