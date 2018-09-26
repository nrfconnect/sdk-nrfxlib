NFC
###

The NFC libraries are RTOS-agnostic NFC libraries built for
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
- Only one NFC library can be used in a single application.
- The libraries require their respective NFC HAL software module that is
  provided as source code in the /nrfxlib/nfc/src folder:

	- libnfct2t_nrf52.a requires hal_nfc_t2t.c
	- libnfct4t_nrf52.a requires hal_nfc_t4t.c
- Each NFC HAL module (hal_nfc_t2t.c or hal_nfc_t4t.c) must be the only user of
  each of the following peripherals:

	- NFCT
	- TIMER4
- The NFC HAL modules start the HFCLK crystal oscillator during initialization,
  and do not stop it.

Disclaimer
**********
- The libraries are for evaluation purposes only.
