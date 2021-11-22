.. _nrf_security_different_tfm_config:

Different configurations in SPE and NSPE
########################################

Normally in a TF-M application with a NSPE image and a SPE image the
NSPE will invoke the PSA Crypto APIs to do crypto. And the feature set
of the PSA Crypto API will be configured through nrf_security. So
there will be no crypto implementations in the NSPE.

Therefore in a normal TF-M application there will be two instances of
nrf_security, one in the NSPE that just contains an API for PSA
Crypto. And one in the SPE which contains a cryptocell driver and
possibly other backends as well. Yet they will both use the same
configuration.

But, if an application needs to have some crypto features implemented
in the NSPE nrf_security instance and other features implemented in
the SPE nrf_security instance the following guide can be followed to
configure each instance independently:

1. Build the application for a board with TF-M.

   .. code-block:: console

      export APP_DIR=~/ncs/nrf/samples/crypto/psa_tls
      cd $APP_DIR
      west build -b nrf9160dk_nrf9160_ns

#. Enable nrf_security, if it is not enabled already, and configure
   the crypto functionality and enabled backends as they should be
   configured in the SPE. See :ref::`_nrf_security_backend_config` for
   configuration of nrf_security.

#. Copy the resulting configuration from the build directory to the
   application source directory. This copy will be used to configure
   the SPE nrf_security instance.

   .. code-block:: console

      cp $APP_DIR/build/zephyr/.config $APP_DIR/secure_zephyr.config
      mkdir -p $APP_DIR/secure/zephyr
      cp $APP_DIR/build/zephyr/include/generated/autoconf.h $APP_DIR/secure/zephyr/

#. Modify the application build scripts such that the SPE nrf_security
   instance uses these copied files to configure itself instead of
   using the same configuration as the NSPE instance.

   .. code-block:: console

      cat ~/ncs/nrf/samples/crypto/nrf_security_snippet.cmake >> $APP_DIR/CMakeLists.txt

#. Rebuild.

   .. code-block:: console

      west build -b nrf9160dk_nrf9160_ns

#. Now, any configuration of nrf_security through Kconfig will only
   affect the NSPE instance. To configure the SPE instance either edit
   the two copied files manually or re-generate a configuration with
   Kconfig and copy the files again.
