#include <Geode/Geode.hpp>
#include <BatteryInfo.hpp>

using namespace geode::prelude;
using namespace arcticwoof;

// Setup the mod
$on_mod(Loaded) {
    // Initialize the singleton when the mod loads
    BatteryInfo::get();
    
    log::info("BatteryInfo API initialized");
}