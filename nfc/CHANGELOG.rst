.. _nfc_changelog:

Changelog
#########

All notable changes to this project are documented in this file.

NFC 0.4.0
****************

Added Type 2 Tag and Type 4 Tag NFC libraries for the nRF5340 device.
Both libraries are compiled for the Cortex-M33 CPU in hard and soft float variants.

Added
=====

* Added the NFC T2T library for the nRF5340 device: ``libnfct2t_nrf53.a``.
* Added the NFC T4T library for the nRF5340 device: ``libnfct4t_nrf53.a``.
* Added a feature to the NFC T4T library that allows the user to change the NDEF
  buffer when the NFC stack is already initialized and running.

NFC 0.3.0
****************

Fixed minor bugs in the NFC libraries.

Modified
========

* Modified the NFC platform module (``nfc_platform_zephyr.c``) to use
  the Zephyr Clock Controller driver.
* Fixed the behavior of the Type 2 Tag library (``libnfct2t_nrf52.a``) when
  receiving an unsupported WRITE command.


NFC 0.2.0
****************

The NFC libraries now use the NFCT driver instead of the NFC HAL modules. The
NFCT driver is a part of nrfx repository.

Added
=====

* Added the NFC platform module to abstract runtime environment specific
  implementation:

  * ``nfc_platform_zephyr.c``

Removed
=======

* Removed the NFC HAL modules (replaced by the NFCT driver):

  * ``hal_nfc_t2t.c``
  * ``hal_nfc_t4t.c``


NFC 0.1.0
*********

Initial release.

Added
=====

* Added the following NFC libraries in both soft-float and hard-float builds:

  * ``libnfct2t_nrf52.a``
  * ``libnfct4t_nrf52.a``
