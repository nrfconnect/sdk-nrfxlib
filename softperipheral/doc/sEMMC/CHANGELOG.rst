.. _semmc_changelog:

sEMMC changelog
###############

.. contents::
   :local:
   :depth: 2

All the notable changes to sEMMC are documented on this page.
Refer to :ref:`intro_soft_peripherals_platform_support` for supported Nordic devices for each version.

v1.0.0
******

See the list of changes for the current release.

.. note::
   This release introduces a change in the API which breaks backward compatibility.

* Added:

  * sEMMC has been extended to support SDCARD. 
  * sEMMC memory map has been extended for H20 target. 

* Updated:

  * API is now more generic to support both EMMC and SDCARD:

    * Transfer descriptor buffers can be ``NULL`` for commands with no associated data. (:c:var:`nrf_semmc_transfer_desc_t.buffer` = ``NULL``).
      Alternatively :c:func:`nrf_semmc_cmd` can take argument ``NULL`` for the transfer descriptor.
    * Transfer descriptors now require a direction argument (:c:var:`nrf_semmc_transfer_desc_t.buffer.transfer_direction`)
    * Adding more response type enums (:c:var:`nrf_semmc_resp_type_t`);
    * Adding convenience function to actively poll DAT0 line, as an alternative to ``R1B`` response type (:c:func:`nrf_semmc_dat0_busy_poll`) a
      command can be issued with ``R1`` response type and then call the convenience function. This is important for SDCARD ``CMD11``.
    * Adding convenience function to actively poll number of transferred blocks in a multi-block transfer (:c:func:`nrf_semmc_blocks_done_get`).

v0.1.1
******

* Fixed firmware bug for the nRF54L Series devices. 

v0.1.0
******

This is an initial release.

* Added the following support for the nRF54H Series and nRF54L Series devices:

  * 1-lane and 4-lane data transmission
  * Class 0 commands - Basic
  * Class 2 commands - Block read
  * Class 4 commands - Block write
  * Class 5 commands - Erase
  * Class 6 commands - Write protection
  * Class 7 commands - Lock device
  * Class 11 commands - Command queuing
  * All response types
