# Changelog
All notable changes to this project are documented in this file.

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
