.. _porting:

Porting to OS
#############

The Remote Procedure Serialization needs port to your operating system or bare-metal application.
You have to provide:

	* Os specific function depends on ``include/rp_os.h``
	* rp_log.h for logging system abstraction according to the template ``template/rp_log.h``
	* One or more transport layer according to the template ``template/rp_trans_tmpl.h``


OS porting Zephyr
=================

Creating instance
-----------------

When creating a new Remote Procedure Serialization instance the locking mechanism data type have to be indicated:

.. code-block:: c

	RP_SER_DEFINE(entropy_ser, struct k_sem, 0, 1000, 0);

Example ``rp_os.c`` source file:

.. literalinclude:: ../../../nrf/subsys/serialization/rp_os.c
    :language: c
