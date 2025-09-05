#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "BatteryInfo.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer)
{
    // Store fields in a Fields struct as per Geode's requirements
    struct Fields {
        CCLabelBMFont* m_batteryLabel = nullptr;
        bool m_isCharging = false;
    };

    void updateBatteryInfo() {
        if (!m_fields->m_batteryLabel) return;

        float batteryLevel = BatteryInfo::getBatteryLevel();
        bool charging = BatteryInfo::isCharging();

        // Only update if battery info changed
        if (batteryLevel >= 0.0f) {
            int batteryPercent = static_cast<int>(batteryLevel);
            std::string displayText = std::to_string(batteryPercent) + "%";
            
            // Add charging indicator if charging
            if (charging) {
                displayText += " [+]";
            }
            
            m_fields->m_isCharging = charging;
            m_fields->m_batteryLabel->setString(displayText.c_str());
            
            // Update color based on battery level
            if (batteryPercent <= 20) {
                m_fields->m_batteryLabel->setColor(ccColor3B{255, 0, 0}); // Red for low battery
            } else if (batteryPercent <= 50) {
                m_fields->m_batteryLabel->setColor(ccColor3B{255, 165, 0}); // Orange for medium battery
            } else {
                m_fields->m_batteryLabel->setColor(ccColor3B{0, 255, 0}); // Green for good battery
            }
        } else {
            m_fields->m_batteryLabel->setString("N/A");
            m_fields->m_batteryLabel->setColor(ccColor3B{255, 255, 255}); // White for unknown
        }
    }

    bool init(GJGameLevel *level, bool useReplay, bool dontCreateObjects)
    {
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        float batteryLevel = BatteryInfo::getBatteryLevel();
        bool charging = BatteryInfo::isCharging();
        m_fields->m_isCharging = charging;

        // Create the battery label
        if (batteryLevel >= 0.0f) {
            log::debug("Battery: {}%  charging={}", batteryLevel, charging);
            std::string displayText = std::to_string(static_cast<int>(batteryLevel)) + "%";
            if (charging) {
                displayText += " [+]";
            }
            m_fields->m_batteryLabel = CCLabelBMFont::create(
                displayText.c_str(),
                "bigFont.fnt");
            
            // Set color based on battery level
            if (batteryLevel <= 20.0f) {
                m_fields->m_batteryLabel->setColor(ccColor3B{255, 0, 0}); // Red for low battery
            } else if (batteryLevel <= 50.0f) {
                m_fields->m_batteryLabel->setColor(ccColor3B{255, 165, 0}); // Orange for medium battery
            } else {
                m_fields->m_batteryLabel->setColor(ccColor3B{0, 255, 0}); // Green for good battery
            }
        } else {
            log::debug("Battery: unknown");
            m_fields->m_batteryLabel = CCLabelBMFont::create("N/A", "bigFont.fnt");
            m_fields->m_batteryLabel->setColor(ccColor3B{255, 255, 255}); // White for unknown
        }
        
        // Position the label
        m_fields->m_batteryLabel->setPosition({winSize.width - 50.f, winSize.height - 15.f});
        m_fields->m_batteryLabel->setScale(0.5f); // Make it smaller
        this->addChild(m_fields->m_batteryLabel, 100); // High z-order to ensure visibility
        
        // Schedule periodic updates
        this->schedule(schedule_selector(MyPlayLayer::onUpdateBatteryInfo), 5.0f); // Update every 5 seconds

        return true;
    }
    
    void onUpdateBatteryInfo(float dt) {
        updateBatteryInfo();
    }
};