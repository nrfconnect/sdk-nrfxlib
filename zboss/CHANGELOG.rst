.. _zboss_changelog:

Changelog
#########

.. contents::
   :local:
   :depth: 2

All notable changes to this project in |NCS| are documented in this file.


Master branch
*************

Changes
=======

* Reduced the number of ZBOSS libraries to a single set.
* Added sources of the BDB and ZCL layers and removed them from the main ZBOSS library.
* Moved scheduler API prototypes to :file:`zb_osif.h`.
* Added API for sending and receiving inter-PAN frames.
* Extended NCP protocol with inter-PAN commands.
* Added bootloader commands in the NCP protocol.
* Updated the ZBOSS stack to version ``3.6.0.0+3.0.0``.
  For detailed information, see `ZBOSS stack release notes`_ for the |NCS|'s Master branch.

nRF Connect SDK v1.5.1
**********************

Changes
=======

* Updated the production ZBOSS stack to version ``3.3.0.7+03_22_2021``.
  This certification-ready version includes several bugfixes.
  For detailed information, see `ZBOSS stack release notes`_ for the |NCS| v1.5.1.

nRF Connect SDK v1.5.0
**********************

Added
=====

* Development ZBOSS stack library version based on the ZBOSS build v3.5.0.0.
  This library version is dedicated for :ref:`NCP development <nrf:ug_zigbee_platform_design_ncp_details>`.
* Development libraries for the Cortex-M33 CPU.

Changes
=======

* Updated the production ZBOSS stack to version ``3.3.0.6+11_30_2020``.
  For detailed release notes for this stack version, see `ZBOSS stack release notes`_ for the |NCS| v1.5.0.

nRF Connect SDK v1.4.0
**********************

Added
=====

* Added ZBOSS changelog.

Changes
=======

* Updated ZBOSS stack to version ``10_06_2020``.
  For detailed release notes for this stack version, see `ZBOSS stack release notes`_ for the |NCS| v1.4.0.
* Unified OSIF layer for LEDs and buttons.
* Updated the ZBOSS release naming convention for nrfxlib commit.
* Changed MAC LL API.
* ZBOSS IO buffer size reconfigured to allow for sending 802.15.4 MAC frame with maximum payload.

Limitations
***********

This sections lists limitations that apply to all versions of nRF Connect SDK.

* On average, ZBOSS alarms last longer by 6.4 percent than Zephyr alarms.
