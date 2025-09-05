#include <Geode/Geode.hpp>
#include <BatteryInfo.hpp>

using namespace geode::prelude;
using namespace arcticwoof;

$on_mod(Loaded) {
    BatteryInfo::get();
    log::info("BatteryInfo API initialized");
}