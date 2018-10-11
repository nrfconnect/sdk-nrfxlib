NFC
###

The following NFC libraries are RTOS-agnostic NFC libraries built for
Nordic Semiconductor nRF52 Series SoCs.

The following libraries are available:

- libnfct2t_nrf52
	- Intended for nRF52832 and nRF52840.
	- Supports NFC-A Type 2 Tag in read-only mode.
- libnfct4t_nrf52
	- Intended for nRF52832 and nRF52840.
	- Supports NFC-A Type 4 Tag.

Each library is distributed in both soft-float and hard-float builds.

Integration notes
*****************
- The libraries require the NFCT driver, which is a part of the nrfx repository.

- The libraries require the NFC Platform software module to operate in different
  runtime environments. This module must implement the following functions:
	- nfc_platform_setup(), called by the NFC libraries at initialization.
          This function sets up clock interface and connects NFCT and TIMER4
          IRQs with their respective IRQ handler functions from nrfx.
	- nfc_platform_event_handler(), called by the NFC libraries to forward
          NFC events received from the NFCT driver. It is necessary to track
          this event flow in order to determine when HFCLK must be running and
          when NFCT peripheral must be activated.

- This module is responsible for activating the NFCT driver when the following
  conditions are fulfilled:
        - NFC field is present.
        - HFCLK is running.

- Exemplary implementation of the NFC Platform module for the Zephyr environment
  is provided in the nfc_platform_zephyr.c file, which is located in the nfc/src
  folder.

- The NFC Platform module for the Zephyr environment starts the HFCLK crystal
  oscillator during initialization, and does not stop it. You can define a more
  sophisticated module for clock management, which activates the HFCLK only for
  the period when NFC field is present.

- Each library must be the only user of each of the following peripherals:

	- NFCT
	- TIMER4

Disclaimer
**********
- The libraries are for evaluation purposes only.
