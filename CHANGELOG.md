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
* Use the libraries for evaluation purposes only.
* The libraries are not fully functional.
* The libraries are not built for performance.  
* The libraries require to be the only user of the following peripherals:
    * RTC0
    * TIMER0
    * RADIO 
    * POWER/CLOCK
    * SWI5
    * RNG

### Added
- Added libnfct2t_nrf52.a and libnfct4t_nrf52.a

#### libnfct2t_nrf52.a, libnfct4t_nrf52.a

The first release of NFC Libraries consists of libnfct2t_nrf52.a and libnfct4t_nrf52.a. The NFC libraries are OS-agnostic and built to run on the nRF52 Series.
This release contains both soft-float and hard-float editions of the following libraries:
* libnfct2t_nrf52.a
    * Supports NFC-A Type 2 Tag in read-only mode.
    * Intended for nRF52832 and nRF52840.
* libnfct4t_nrf52.a
    * Supports NFC-A Type 4 Tag.
    * Intended for nRF52832 and nRF52840.

Notes: 
* Use the libraries for evaluation purposes only.
* Only one NFC library can be used in a single application.
* The libraries require the NFC HAL software modules that are provided as source code in the /nrfxlib/nfc/src folder:
    * libnfct2t_nrf52.a requires hal_nfc_t2t.c
    * libnfct4t_nrf52.a requires hal_nfc_t4t.c
* Each NFC HAL module (hal_nfc_t2t.c or hal_nfc_t4t.c) requires to be the only user of the following peripherals:
    * NFCT
    * TIMER4
* The NFC HAL modules start the HFCLK crystal oscillator during initialization, and do not stop it.
