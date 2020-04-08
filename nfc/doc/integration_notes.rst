.. _nfc_integration_notes:

Integration notes
#################

* The libraries require the NFCT driver that is a part of the nrfx repository.

* The libraries require the NFC Platform software module to operate in different runtime environments.
  This module must implement the following functions:

  * nfc_platform_setup(), called by the NFC libraries at initialization.
    This function sets up the clock interface and connects NFCT and chosen TIMER IRQs with their respective IRQ handler functions from nrfx.
  * nfc_platform_event_handler(), called by the NFC libraries to forward NFC events received from the NFCT driver.
    It is necessary to track this event flow in order to determine when HFCLK must be running and when it can be stopped.
    The NFCT peripheral can be activated only when HFCLK is running.

    It is recommended to request HFCLK asynchronously with a notification when the clock has been started, and activate the NFCT peripheral after receiving this notification.

* This module is responsible for activating the NFCT driver when the following conditions are fulfilled:

  * NFC field is present.
  * HFCLK is running.

* Exemple implementation of the NFC Platform module for the Zephyr environment is provided in the nRF Connect SDK :file:`subsys/nfc/lib/platform.c` file.

* The NFC Platform module for the Zephyr environment starts the HFCLK crystal oscillator when it has received the NFC field detect event, and stops it after receiving the field lost event.
  HFCLK is activated only for the time when NFC field is present.

* Each library must be the only user of each of the following peripherals:

  * NFCT
  * Chosen TIMER instance
