.. _nrf_security_adv_backend_config:

Advanced backend configuration
##############################

.. contents::
   :local:
   :depth: 2

The advanced configuration section in Kconfig can be used to fine-tune the build of specific backends.
For example, the options available in the advanced configuration section can help to reduce the memory usage and/or flash footprint of the :ref:`nrf_security_backends_orig_mbedtls`.

Actual size reductions depend on the option being adjusted.

Before modifying the default settings, see this `article on reducing mbed TLS memory and storage footprint`_.

.. note::
   The settings available in the advanced configuration section are not validated.
   Adjust these settings with caution.


AES tables in ROM
*****************

You can set the :option:`CONFIG_MBEDTLS_AES_ROM_TABLES` Kconfig variable to place the AES lookup tables in ROM instead of RAM.
This decreases the RAM usage by about 8 KB with an additional cost of about 8 KB of ROM.
Note that executing operations in ROM is slower.
If the configuration :option:`CONFIG_MBEDTLS_AES_FEWER_TABLES` is set, then the size moved from RAM to ROM is about 2 KB.

+------------------------------------------------+---------+-------+-----+
| Option                                         | Default | Min   | Max |
+================================================+=========+=======+=====+
| :option:`CONFIG_MBEDTLS_AES_ROM_TABLES`        | `n`     | `n`   | `y` |
+------------------------------------------------+---------+-------+-----+

.. note::
   This configuration is only available in :ref:`nrf_security_backends_orig_mbedtls`.

Fewer AES tables
****************

The :option:`CONFIG_MBEDTLS_AES_FEWER_TABLES` Kconfig variable controls the size of the AES lookup tables in use.
Enabling this omits about 75% of the AES tables in RAM or ROM.
If the option is enabled, AES must perform more calculations, which impacts the overall performance.

+------------------------------------------------+---------+-------+-----+
| Option                                         | Default | Min   | Max |
+================================================+=========+=======+=====+
| :option:`CONFIG_MBEDTLS_AES_FEWER_TABLES`      | `n`     | `n`   | `y` |
+------------------------------------------------+---------+-------+-----+

.. note::
   This configuration is only available in :ref:`nrf_security_backends_orig_mbedtls`.


Multiple Precision Integers (MPI) / Bignum calculation
******************************************************

The :option:`CONFIG_MBEDTLS_MPI_WINDOW_SIZE` Kconfig variable controls the window size used in mbed TLS for MPI calculations.
Reduce this value to reduce memory usage. Note that reducing this this value may have an impact on the performance.

The :option:`CONFIG_MBEDTLS_MPI_MAX_SIZE` Kconfig variable controls the maximum size of MPIs that can be used for calculation.
Reduce this value only if you are sure that the system will not need larger sizes.


+------------------------------------------------+---------+-------+------+
| Option                                         | Default | Min   | Max  |
+================================================+=========+=======+======+
| :option:`CONFIG_MBEDTLS_MPI_WINDOW_SIZE`       | 6       | 1     | 6    |
+------------------------------------------------+---------+-------+------+
| :option:`CONFIG_MBEDTLS_MPI_MAX_SIZE`          | 1024    | 0     | 1024 |
+------------------------------------------------+---------+-------+------+

.. note::
   This configuration is only available in cc310 backend and :ref:`nrf_security_backends_orig_mbedtls`.


Elliptic Curves
***************

The :option:`CONFIG_MBEDTLS_ECP_MAX_BITS` Kconfig variable controls the largest elliptic curve supported in the library.

If the curves that are used are smaller than 521 bits, then this option can be reduced in order to save memory.
See :ref:`nrf_security_backend_config_ecc_curves` for information on how to select the curves to use.
For example, if `NIST secp384r1` is the only curve enabled, then :option:`CONFIG_MBEDTLS_ECP_MAX_BITS` can be reduced to 384 bits.

The :option:`CONFIG_MBEDTLS_ECP_WINDOW_SIZE` Kconfig variable controls the window size used for elliptic curve multiplication.
This value can be reduced down to 2 to reduce memory usage.
Keep in mind that reducing the value impacts the performance of the system.

The :option:`CONFIG_MBEDTLS_ECP_FIXED_POINT_OPTIM` Kconfig variable controls ECP fixed point optimizations.
If disabled, the system uses less memory, but performance of the system is reduced.


+------------------------------------------------+---------+-------+-----+
| Option                                         | Default | Min   | Max |
+================================================+=========+=======+=====+
| :option:`CONFIG_MBEDTLS_ECP_MAX_BITS`          | 521     | 0     | 521 |
+------------------------------------------------+---------+-------+-----+
| :option:`CONFIG_MBEDTLS_ECP_WINDOW_SIZE`       | 6       | 2     | 6   |
+------------------------------------------------+---------+-------+-----+
| :option:`CONFIG_MBEDTLS_ECP_FIXED_POINT_OPTIM` | `y`     | `n`   | `y` |
+------------------------------------------------+---------+-------+-----+

.. note::
   This configuration is only available in cc310 backend and :ref:`nrf_security_backends_orig_mbedtls`.


SHA-256
*******

The :option:`CONFIG_MBEDTLS_SHA256_SMALLER` Kconfig variable can be used to select a SHA-256 implementation with smaller footprint.
Such configuration reduces SHA-256 calculation performance.

For example, on a Cortex-M4, the size of :cpp:func:`mbedtls_sha256_process()` is reduced from ~2 KB to ~0.5 KB, however it also performs around 30% slower.

+------------------------------------------------+---------+-------+-----+
| Option                                         | Default | Min   | Max |
+================================================+=========+=======+=====+
| :option:`CONFIG_MBEDTLS_SHA256_SMALLER`        | `n`     | `n`   | `y` |
+------------------------------------------------+---------+-------+-----+

.. note::
   This configuration is only available in :ref:`nrf_security_backends_orig_mbedtls`.

SSL Configurations
******************

The :option:`CONFIG_MBEDTLS_SSL_MAX_CONTENT_LEN` Kconfig variable can be used to specify the maximum size for incoming and outgoing mbed TLS I/O buffers.
The default value is 16384 as specified in RFC5246, however if both sides are under your control, this value can safely be reduced under the following conditions:

* Both sides support the max_fragment_length SSL extension, RFC8449.
  The max_fragment_length allows for buffer reduction to less than 1 KB.
* Knowledge of the maximum size that will ever be sent in a single SSL/TLS frame.

If one of those conditions is met, the buffer size can safely be reduced to a more appropriate value for memory constrained devices.

The :option:`CONFIG_MBEDTLS_SSL_CIPHERSUITES` Kconfig variable is a custom list of cipher suites to support in SSL/TLS.
The cipher suites are provided as a comma-separated string, in order of preference.
This list can only be used for restricting cipher suites available in the system.


+------------------------------------------------+---------+-----------+-------+-------+
| Option                                         | Type    | Default   | Min   | Max   |
+================================================+=========+===========+=======+=======+
| :option:`CONFIG_MBEDTLS_SSL_MAX_CONTENT_LEN`   | Integer | 16384     | 0     | 16384 |
+------------------------------------------------+---------+-----------+-------+-------+
| :option:`CONFIG_MBEDTLS_SSL_CIPHERSUITES`      | String  | `<empty>` |       |       |
+------------------------------------------------+---------+-----------+-------+-------+

.. note::
   The string in :option:`CONFIG_MBEDTLS_SSL_CIPHERSUITES` should not be quoted.
