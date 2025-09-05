#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "BatteryInfo.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer)
{

    bool init(GJGameLevel *level, bool useReplay, bool dontCreateObjects)
    {

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        float batteryLevel = BatteryInfo::getBatteryLevel();
        bool charging = BatteryInfo::isCharging();

        if (batteryLevel >= 0.0f)
        {
            log::debug("Battery: {}%  charging={}", batteryLevel, charging);
            auto label = CCLabelBMFont::create(
                (std::to_string(static_cast<int>(batteryLevel)) + "%").c_str(),
                "bigFont.fnt");
            label->setPosition({winSize.width, winSize.height});
            this->addChild(label);
        }
        else
        {
            log::debug("Battery: unknown");
            auto label = CCLabelBMFont::create(
                "N/A",
                "bigFont.fnt");
            label->setPosition({winSize.width, winSize.height});
            this->addChild(label);
        }

        return true;
    }
};