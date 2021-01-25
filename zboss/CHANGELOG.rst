.. _zboss_changelog:

Changelog
#########

.. contents::
   :local:
   :depth: 2

All notable changes to this project in |NCS| are documented in this file.

Master branch
*************

Added
=====

* Additional ZBOSS stack library version based on the ZBOSS build v3.4.0.800.
  This library version is dedicated for :ref:`NCP development <nrf:ug_zigbee_platform_design_ncp_details>`.

Changes
=======

* Updated ZBOSS stack to version ``3.3.0.6+11_30_2020`` that includes the Cortex-M33 CPU libraries.
  For detailed release notes for this stack version, see `ZBOSS stack release notes`_ for the Master branch.

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
