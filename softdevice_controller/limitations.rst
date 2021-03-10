.. softdevice_controller_limitations:

Limitations
###########

.. contents::
   :local:
   :depth: 2

DRGN-8628: Setting Filter_Duplicates in LE Set Scan Enable and LE Set Extended Scan Enable commands does not take effect
  The host will receive all advertisements from advertisers in advertising report events.

  **Workaround:** The duplicate advertising reports can be filtered out by the application.
