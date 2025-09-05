#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "BatteryInfo.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {

    if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        float batteryLevel = BatteryInfo::getBatteryLevel();
        bool charging = BatteryInfo::isCharging();

        if (batteryLevel >= 0.0f) {
            log::debug("Battery: %.0f%%  charging=%d", batteryLevel, charging);
        } else {
            log::debug("Battery: unknown");
        }

        return true;
    }
};