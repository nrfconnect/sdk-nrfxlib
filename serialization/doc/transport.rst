.. _transport:

Transport
#########

The RP SER library can use any transport for communication with the remote CPU. This layer is fully hardware and OS dependent. The default transport is
`OpenAMP <https://github.com/OpenAMP/open-amp/>`_. User can define it own transport using template. Tranport layer should be a part of yours operating system.

Template header:
``template/rp_transp_tmpl.h``

Transport layer configuration is located in ``include/rp_trans.h``, where you can add your own transport or choose one of the available implementations.
