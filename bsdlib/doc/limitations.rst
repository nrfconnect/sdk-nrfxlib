.. _limitations:

Limitations
###########

GNSS, AGPS, and modem DFU are not part of the generic sockets.
Therefore, operating on them using ``poll`` is not possible.

PDN sockets only support ``open``, ``close``, ``setsockopt``, and ``getsockopt`` to configure a PDN with an APN name.
Therefore, there is no data communication on the socket, and it is not pollable.


Modem firmware limitations
**************************

For a detailed description of limitations related to the modem firmware used, refer to the changelog of that specific version.
The changelog is distributed as part of the `downloadable zip file`_.

.. _downloadable zip file: https://www.nordicsemi.com/Products/Low-power-cellular-IoT/nRF9160/Download#98C9E2578566420786ABD40B695FDB9B
