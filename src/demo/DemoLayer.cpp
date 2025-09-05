#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <BatteryInfo.hpp>

using namespace geode::prelude;
using namespace arcticwoof;

// This is a demo class showing how to use the Battery Info API
class $modify(MyPlayLayer, PlayLayer)
{
    bool init(GJGameLevel *level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        auto batteryAPI = BatteryInfo::get();
        float batteryLevel = batteryAPI->getBatteryLevel();
        bool charging = batteryAPI->isCharging();
        bool batterySaver = batteryAPI->isBatterySaver();

        log::debug("Battery level: {}%, Charging: {}, Battery Saver: {}",
                   batteryLevel, charging ? "Yes" : "No", batterySaver ? "On" : "Off");

        // Only show if battery level is known and demo is enabled
        if (batteryLevel >= 0.0f && Mod::get()->getSettingValue<bool>("demo"))
        {

            auto existingLabel = this->getChildByTag(1001);
            if (existingLabel)
            {
                // Label already exists, let's update it
                updateBatteryDisplay(0.0f);
            }
            else
            {
                std::string statusText = "Battery: " + std::to_string(static_cast<int>(batteryLevel)) + "%";
                
                // Add charging status if applicable
                if (charging) {
                    statusText += " [Charging]";
                }
                
                // Add battery saver status independently
                if (batterySaver) {
                    statusText += " [Saver]";
                }

                auto label = CCLabelBMFont::create(statusText.c_str(), "bigFont.fnt");
                label->setScale(0.5f);

                // Position at top middle
                auto winSize = CCDirector::sharedDirector()->getWinSize();
                label->setPosition({winSize.width / 2, winSize.height - 25});

                // Set color based on battery level
                if (batteryLevel <= 20.0f)
                {
                    label->setColor(ccColor3B{255, 0, 0}); // Red for low battery
                }
                else if (batteryLevel <= 50.0f)
                {
                    label->setColor(ccColor3B{255, 165, 0}); // Orange for medium battery
                }
                else
                {
                    label->setColor(ccColor3B{0, 255, 0}); // Green for good battery
                }

                label->setTag(1001);
                this->addChild(label);

                // Schedule a periodic update to refresh battery info
                this->schedule(schedule_selector(MyPlayLayer::updateBatteryDisplay), 5.0f);
            }
        }

        return true;
    }

    void updateBatteryDisplay(float dt)
    {
        auto batteryAPI = BatteryInfo::get();
        float lvl = batteryAPI->getBatteryLevel();
        bool chrg = batteryAPI->isCharging();
        bool saver = batteryAPI->isBatterySaver();

        auto label = static_cast<CCLabelBMFont *>(this->getChildByTag(1001));
        if (!label)
            return;

        if (lvl >= 0.0f)
        {
            // Create base text with battery level
            std::string text = "Battery: " + std::to_string(static_cast<int>(lvl)) + "%";
            
            // Add charging status if applicable
            if (chrg) {
                text += " [Charging]";
            }
            
            // Add battery saver status independently
            if (saver) {
                text += " [Saver]";
            }
            
            label->setString(text.c_str());

            // Update color
            if (lvl <= 20.0f)
            {
                label->setColor(ccColor3B{255, 0, 0});
            }
            else if (lvl <= 50.0f)
            {
                label->setColor(ccColor3B{255, 165, 0});
            }
            else
            {
                label->setColor(ccColor3B{0, 255, 0});
            }
        }
    }
};
