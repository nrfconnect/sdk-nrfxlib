.. _nrf_security_different_tfm_config:

Different configurations in SPE and NSPE
########################################

When a TF-M application has both an NSPE (Non-secure Processing Environment) image and an SPE (Secure Processing Environment) image, the NSPE usually invokes the PSA Cryptography APIs to perform cryptographic operations.
In this case, you can configure the feature set of the PSA Cryptography API through the Nordic Security Module (nrf_security).
As such, there are no cryptographic implementations in the NSPE.

Therefore, in a typical TF-M application, there are two instances of the Nordic Security Module:

* One instance in the NSPE that contains just an API for the PSA Cryptography.
* One instance in the SPE that contains an ARM Cryptocell driver and possibly other backends.

Both instances would still use the same configuration.

If you want to independently configure the two Nordic Security Module instances so that they can have different features, follow the steps below:

1. Build the application for a development kit with TF-M:

   .. code-block:: console

      export APP_DIR=~/ncs/nrf/samples/crypto/psa_tls
      cd $APP_DIR
      west build -b nrf9160dk_nrf9160_ns

#. Enable the Nordic Security Module, if it is not enabled, and configure the cryptographic functionality and the necessary backends as needed by the SPE.
   See :ref::`_nrf_security_backend_config` for details on the configuration of the Nordic Security Module.

#. Copy the resulting configuration files from the build directory to the application source directory:

   .. code-block:: console

      cp $APP_DIR/build/zephyr/.config $APP_DIR/secure_zephyr.config
      mkdir -p $APP_DIR/secure/zephyr
      cp $APP_DIR/build/zephyr/include/generated/autoconf.h $APP_DIR/secure/zephyr/

   The build system can now use this copy to configure the SPE nrf_security instance.

#. Modify the application build scripts to let the SPE nrf_security instance use these copied files to configure itself instead of using the same configuration as the NSPE instance:

   .. code-block:: console

      cat ~/ncs/nrf/samples/crypto/nrf_security_snippet.cmake >> $APP_DIR/CMakeLists.txt

#. Rebuild the application:

   .. code-block:: console

      west build -b nrf9160dk_nrf9160_ns

After these steps, configuring the Nordic Security Module through Kconfig options would only affect the NSPE instance.
To configure the SPE instance, either edit the copied files or re-generate a configuration with Kconfig and copy the files again.
