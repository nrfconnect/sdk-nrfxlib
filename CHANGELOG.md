# Changelog
All notable changes to this project are documented in this file.

### Added
- Added ble_controller_nrf52_0.1.0-1.prealpha

#### ble_controller_nrf52_0.1.0-1.prealpha

The ble-controller 0.1.0-1.prealpha is the first ble-controller release. The ble-controller libraries are OS-agnostic and built to run on the nRF52 Series. 
This release contains both soft-float, softfp-float and hard-float editions of the following libraries:
* libble_controller_s112_nrf52_0.1.0-1.prealpha.a
    * Supports peripheral role only.
    * Intended for nRF52810 and nRF52832.
* llibble_controller_s132_nrf52_0.1.0-1.prealpha.a
    * Supports both peripheral and central roles.
    * Intended for nRF52810 and nRF52832.
* libble_controller_s140_nrf52_0.1.0-1.prealpha.a
    * Supports both peripheral and central roles.
    * Supports Coded PHY.
    * Intended for nRF52840.

Each of the libraries support a fixed configuration: two central links and one peripheral link.
Notes: 
* The libraries should only be used for evaluation purposes.
* The libraries are not fully functional.
* The libraries are not built for performance.  
* The libraries require to be the only user of the following peripherals:
    * RTC0
    * TIMER0
    * RADIO 
    * POWER/CLOCK
    * SWI5
    * RNG
