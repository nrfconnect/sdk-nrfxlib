.. _mpsl_pm:

MPSL power management
#####################

.. contents::
   :local:
   :depth: 2

The power management module enables the use of MPSL in a power-efficient way on the nRF54H20 Series SoCs.

Overview
********

The MPSL event scheduling system has the following requirements:

* ``RADIO`` events are started on time.
  The system must prepare itself, for example, by waking up earlier to handle scheduled events precisely.
* There is no latency during ``RADIO`` events.
  The system must ensure that its hardware and software components, which may affect event execution time, are configured to not introduce latency.
  For example, required power domains are always powered, and memory that stores instructions is always powered.

Meeting these requirements depends on a particular system.

The power management module provides APIs that can be used to get information to configure wake-up time of the system and low latency in the system.

The MPSL notifies the system about new scheduled events, the end of an event, and low latency mode changes by signaling a low-priority interrupt.
When this interrupt is triggered, you must handle changes in the MPSL power management parameters and low latency requests as soon as possible.

Event parameters update
***********************

The module provides the :c:func:`mpsl_pm_params_get` function, which returns information about the current power management module parameters related to event scheduling.
You must check the following conditions before you apply new parameters:

* The :c:func:`mpsl_pm_params_get` function's return value equals ``true``.
* The :c:member:`cnt_flag` value was incremented since the last parameters check.

.. note::
    The MPSL power management module holds an internal counter that is incremented every time there is a change in the parameters.
    The low-priority handler should store its own copy of the counter to determine if the returned parameters were updated.
    The counter may be incremented more than once since last check, but the parameters only store the most recent values.
    Always update the local copy of the counter to match the most recent :c:member:`cnt_flag` value.

The following is a template for a parameters update handler:

.. code-block:: console

    static uint8_t m_pm_prev_flag_value;

    static void m_register_event(void)
    {
        mpsl_pm_params_t params = {0};
        bool pm_param_valid = mpsl_pm_params_get(&params);

        if (m_pm_prev_flag_value == params.cnt_flag) {
            /* We have no new information to process. */
            return;
        }
        if (!pm_param_valid) {
            /* High priority did change mpsl_pm_params, while the function call. */
            m_pm_prev_flag_value = params.cnt_flag;
            return;
        }
        switch (params.event_state) {
            case MPSL_PM_EVENT_STATE_NO_EVENTS_LEFT:
            {
                /* Unregister an event here. */
                break;
            }
            case MPSL_PM_EVENT_STATE_BEFORE_EVENT:
            {
                /* Register of update an event here. */
                break;
            }
            default:
                __ASSERT(false, "MPSL PM is in an undefined state.");
        }

        m_pm_prev_flag_value = params.cnt_flag;
    }

Low latency request
*******************

Low latency request or release may be long-running operations that should be divided into a change trigger and a completion handler.
The processing of long-running latency change operations may be interrupted by MPSL high-priority context.
The interrupt may change the low latency request.
To avoid race conditions, coordination between the low latency request handler and the MPSL power management module is required.

The state of the system regarding the low latency request is stored in the MPSL power management module.
The information about the low latency state is provided by the :c:func:`mpsl_pm_low_latency_state_get` function and it may be changed by the :c:func:`mpsl_pm_low_latency_state_set` function.
The use of the :c:func:`mpsl_pm_low_latency_state_set` function is not allowed in the MPSL.

The information on whether low latency is requested is provided by the :c:func:`mpsl_pm_low_latency_requested` function.
This change must be applied to the system as soon as possible, to make sure it is completed before an event starts.

.. note::
    If the low latency request is not applied before an event starts, the event will be aborted.

Only the MPSL will change the value returned by the :c:func:`mpsl_pm_low_latency_requested` function.

There are fours states for the low latency request described by :c:enum:`mpsl_pm_low_latency_state_t`.
They reflect the state of a system reconfiguration according to the recent request.
Only a low latency request handler can update the state.

The low latency request may be started only in the :c:enumerator:`MPSL_PM_LOW_LATENCY_STATE_OFF` state.
The low latency release may be started only in the :c:enumerator:`MPSL_PM_LOW_LATENCY_STATE_ON` state.

The remaining states are used to inform MPSL power management that the system is in progress of a low latency update.
No other operations are allowed in these states.

The following is a template for a low latency request handler:

.. code-block:: console

    static void m_register_latency(void)
    {
        switch (mpsl_pm_low_latency_state_get()) {
        case MPSL_PM_LOW_LATENCY_STATE_OFF:
            if (mpsl_pm_low_latency_requested()) {
                mpsl_pm_low_latency_state_set(MPSL_PM_LOW_LATENCY_STATE_REQUESTING);

                /* Start low latency request in the system */
            }
            break;
        case MPSL_PM_LOW_LATENCY_STATE_ON:
            if (!mpsl_pm_low_latency_requested()) {
                mpsl_pm_low_latency_state_set(MPSL_PM_LOW_LATENCY_STATE_RELEASING);

                /* Start low latency release in the system
                 *
                 * Let us assume the release operation is short and you do not use completion handler
                 */
                mpsl_pm_low_latency_state_set(MPSL_PM_LOW_LATENCY_STATE_OFF);
            }
            break;
        default:
            break;
        }
    }

    static void m_low_latency_request_cb(int res)
    {
        if (res < 0) {
            __ASSERT(false, "MRAM low latency request could not be handled, reason: %d", res);
            return;
        }

        if (mpsl_pm_low_latency_state_get() == MPSL_PM_LOW_LATENCY_STATE_REQUESTING) {
            mpsl_pm_low_latency_state_set(MPSL_PM_LOW_LATENCY_STATE_ON);
        }
    }
