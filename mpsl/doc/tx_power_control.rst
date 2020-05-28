.. _mpsl_tx_power_control:

TX Power Control
################

The TX Power control feature allows the application to set a maximum TX Power envelope.
That is, the application can limit the TX power to be used per PHY and per channel.
Limiting the Radio TX power on edge bands may be necessary to pass teleregulatory requirements
when using an external amplifier.

Radio protocols may define their own TX power levels for different types of radio activity.
However, when utilizing the MPSL TX Power feature, the actual Radio TX Power may be limited.

To ensure that the feature behaves in a consistent way, the TX Power channel maps shall always be configured when the MPSL is in an idle state with no active users.
