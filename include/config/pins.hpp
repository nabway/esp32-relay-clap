#pragma once
/**
 * @file pins.hpp
 * @brief Single source of truth for all GPIO assignments.
 *
 * Never hardcode pin numbers elsewhere. If a pin must change,
 * this is the only file to touch.
 */

namespace pins {

constexpr int ONBOARD_LED = 2;

}  // namespace pins
