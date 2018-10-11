Changelog
#########
All notable changes to this project are documented in this file.

NFC [Unreleased]
****************

The NFC libraries now use the NFCT driver instead of the NFC HAL modules. The
NFCT driver is a part of nrfx repository.

Added
=====
+ Added the NFC platform module to abstract runtime environment specific
  implementation:
        + nfc_platform_zephyr.c

Removed
=======
+ Removed the NFC HAL modules (replaced by the NFCT driver):
        + hal_nfc_t2t.c
        + hal_nfc_t4t.c

NFC 0.1.0
*********

Initial release.

Added
=====
+ Added the following NFC libraries in both soft-float and hard-float builds.
	+ libnfct2t_nrf52.a
	+ libnfct4t_nrf52.a
