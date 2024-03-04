/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem_gnss.h
 *
 * @defgroup nrf_modem_gnss GNSS API
 * @{
 * @brief API for accessing the GNSS module on the modem.
 */
#ifndef NRF_MODEM_GNSS_H__
#define NRF_MODEM_GNSS_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** The maximum number of positioning systems supported by the interface (GPS, QZSS, etc.). */
#define NRF_MODEM_GNSS_MAX_SYSTEMS 4

/** The maximum number of positioning signals supported by the interface (GPS L1 C/A,
 *  QZSS L1 C/A, etc.).
 */
#define NRF_MODEM_GNSS_MAX_SIGNALS 4

/** @brief Maximum number of satellites that can be tracked at the same time. */
#define NRF_MODEM_GNSS_MAX_SATELLITES 12

/** The number of GPS satellites. */
#define NRF_MODEM_GNSS_NUM_GPS_SATELLITES 32

/** @defgroup nrf_modem_gnss_system_id GNSS system IDs
 *
 * @brief Used to indicate the GNSS system type.
 * @{
 */
/** @brief Invalid. */
#define NRF_MODEM_GNSS_SYSTEM_INVALID 0
/** @brief GPS. */
#define NRF_MODEM_GNSS_SYSTEM_GPS     1
/** @brief QZSS. */
#define NRF_MODEM_GNSS_SYSTEM_QZSS    3
/** @} */

/** @defgroup nrf_modem_gnss_signal_id GNSS signal IDs
 *
 * @brief Used to indicate the GNSS signal type.
 * @{
 */
/** @brief Invalid. */
#define NRF_MODEM_GNSS_SIGNAL_INVALID    0
/** @brief GPS L1 C/A. */
#define NRF_MODEM_GNSS_SIGNAL_GPS_L1_CA  1
/** @brief QZSS L1 C/A. */
#define NRF_MODEM_GNSS_SIGNAL_QZSS_L1_CA 3
/** @} */

/** @defgroup nrf_modem_gnss_signal_bitmask GNSS signal bitmask values
 *
 * @brief Use these bitmask values to enable different GNSS signals.
 *
 * @details The values can be OR'ed together to enable multiple GNSS signals at the same time. If a
 *          signal bit is 0, the corresponding signal is disabled.
 * @{
 */
/** @brief Global Positioning System (GPS) L1 C/A.
 *
 * @note GPS L1 C/A can't be disabled and remains enabled even if the corresponding bit is not set.
 */
#define NRF_MODEM_GNSS_SYSTEM_GPS_L1_CA_MASK  0x01
/** @brief Quasi-Zenith Satellite System (QZSS) L1 C/A. */
#define NRF_MODEM_GNSS_SYSTEM_QZSS_L1_CA_MASK 0x04
/** @} */

/** @defgroup nrf_modem_gnss_nmea_string_bitmask NMEA string bitmask values
 *
 * @brief Use these bitmask values to enable different type of NMEA output strings.
 *
 * @details The values can be OR'ed together to enable multiple string types at the same time.
 *          Writing 0 to the bit position will disable the corresponding NMEA string type.
 * @{
 */
/** @brief Enables Global Positioning System Fix Data. */
#define NRF_MODEM_GNSS_NMEA_GGA_MASK 0x01
/** @brief Enables Geographic Position Latitude/Longitude and time. */
#define NRF_MODEM_GNSS_NMEA_GLL_MASK 0x02
/** @brief Enables DOP and active satellites. */
#define NRF_MODEM_GNSS_NMEA_GSA_MASK 0x04
/** @brief Enables Satellites in view. */
#define NRF_MODEM_GNSS_NMEA_GSV_MASK 0x08
/** @brief Enables Recommended minimum specific GPS/Transit data. */
#define NRF_MODEM_GNSS_NMEA_RMC_MASK 0x10
/** @} */

/** @defgroup nrf_modem_gnss_power_save_modes Power save mode enumerator
 *
 * @brief Use these values to select which power save mode GNSS should use. This only affects
 *        continuous navigation mode. When GNSS engages duty-cycled tracking, it only tracks for
 *        20% of time and spends the rest of the time in sleep. The different modes control how
 *        aggressively GNSS engages duty-cycled tracking, but the duty-cycling itself is the same
 *        with both modes.
 * @{
 */
/** @brief No power save mode is enabled. */
#define NRF_MODEM_GNSS_PSM_DISABLED                 0
/** @brief Enables duty-cycling performance policy power save mode. */
#define NRF_MODEM_GNSS_PSM_DUTY_CYCLING_PERFORMANCE 1
/** @brief Enables duty-cycling power policy power save mode. */
#define NRF_MODEM_GNSS_PSM_DUTY_CYCLING_POWER       2
/** @} */

/** @defgroup nrf_modem_gnss_use_case_bitmask Use case bitmask values
 *
 * @brief Use these bit values to select which use case mode GNSS should use. A use case mode is a
 *        combination of the values of all of the bits.
 * @{
 */
/** @brief Multiple hot starts performance mode.
 *
 * @note This is the only start mode currently supported, but the bit should always be set when
 *       setting the use-case modes to keep the code compliant with future versions of the modem
 *       firmware.
 */
#define NRF_MODEM_GNSS_USE_CASE_MULTIPLE_HOT_START     0x01
/** @brief Low accuracy fixes allowed.
 *
 * @details The error in position calculation can be larger than in normal accuracy mode. In
 *          addition, GNSS might only use three satellites to determine a fix, while in normal
 *          accuracy mode at least four satellites are used.
 */
#define NRF_MODEM_GNSS_USE_CASE_LOW_ACCURACY	       0x02
/** @brief Disable scheduled downloads.
 *
 * @details By default, in periodic navigation mode, when GNSS determines it needs to download
 *          ephemerides or almanacs from the broadcast, the fix interval and fix retry parameters
 *          are temporarily ignored. GNSS will perform scheduled downloads until it has downloaded
 *          the data it needs, after which normal operation is resumed.
 *
 * When this bit is set, scheduled downloads are disabled. This is recommended when A-GNSS is used
 * to supply assistance data to the GNSS. It is also possible to use this option without A-GNSS,
 * but it should be noted that in that case GNSS will never get some data (for example ionospheric
 * corrections), which may affect the accuracy.
 *
 * This option is only valid in periodic navigation mode, because scheduled downloads are not
 * performed in other navigation modes.
 */
#define NRF_MODEM_GNSS_USE_CASE_SCHED_DOWNLOAD_DISABLE 0x04
/** @} */

/** @defgroup nrf_modem_gnss_pvt_flag_bitmask Bitmask values for flags in the PVT notification
 *
 * @brief Bitmask values for the flags member in the #nrf_modem_gnss_pvt_data_frame.
 * @{
 */
/** @brief A valid fix has been acquired. */
#define NRF_MODEM_GNSS_PVT_FLAG_FIX_VALID              0x01
/** @brief The leap second information is valid.
 *
 * @details This bit in the notification flags tells if receiver has decoded leap second from the
 *          navigation message. The leap second is needed for determining GPS-UTC time offset (in
 *          seconds). If it is not decoded (bit is zero), the value of 18 seconds is used. This is
 *          the effective value since January 1st 2017.
 */
#define NRF_MODEM_GNSS_PVT_FLAG_LEAP_SECOND_VALID      0x02
/** @brief At least one sleep period has passed since the last PVT notification.
 *
 * @details Indicates sleep periods during duty-cycled tracking.
 */
#define NRF_MODEM_GNSS_PVT_FLAG_SLEEP_BETWEEN_PVT      0x04
/** @brief Notification deadline was missed.
 *
 * @details Indicates that GNSS has not received a time window since the last PVT notification due
 *          to LTE activity, blocking GNSS operation.
 */
#define NRF_MODEM_GNSS_PVT_FLAG_DEADLINE_MISSED        0x08
/** @brief Average time windows granted to GNSS may be too short.
 *
 * @details Indicates that the inability to produce a valid PVT estimate is likely caused by too
 *          short GNSS time windows. Usually this happens when eDRX and/or PSM are not used, because
 *          LTE needs to monitor pagings from the network with a short interval.
 */
#define NRF_MODEM_GNSS_PVT_FLAG_NOT_ENOUGH_WINDOW_TIME 0x10
/** @brief The velocity estimate is valid. */
#define NRF_MODEM_GNSS_PVT_FLAG_VELOCITY_VALID         0x20
/** @brief The PVT notification was sent because of a scheduled download.
 *
 * @details Indicates that GNSS is running because of a scheduled download. This flag is only used
 *          in the periodic navigation mode.
 *
 * @note This flag is only supported by modem firmware v2.0.0 or later.
 */
#define NRF_MODEM_GNSS_PVT_FLAG_SCHED_DOWNLOAD         0x40
/** @} */

/** @defgroup nrf_modem_gnss_sv_flag_bitmask Satellite flags bitmask values
 *
 * @brief Bitmask values for the flags member in the SV elements in the PVT notification struct.
 * @{
 */
/** @brief The satellite is used in the position calculation. */
#define NRF_MODEM_GNSS_SV_FLAG_USED_IN_FIX 0x02
/** @brief The satellite is unhealthy. */
#define NRF_MODEM_GNSS_SV_FLAG_UNHEALTHY   0x08
/** @} */

/** @defgroup nrf_modem_gnss_agnss_data_type A-GNSS data type enumerator
 *
 * @brief Use these values to indicate the type of A-GNSS data written using
 *        #nrf_modem_gnss_agnss_write.
 * @{
 */
/** @brief GPS UTC assistance A-GNSS parameters.
 *
 * @details Data model #nrf_modem_gnss_agnss_gps_data_utc.
 */
#define NRF_MODEM_GNSS_AGNSS_GPS_UTC_PARAMETERS               1
/** @brief GPS/QZSS ephemeris assistance A-GNSS parameters.
 *
 * @details Data model #nrf_modem_gnss_agnss_gps_data_ephemeris.
 */
#define NRF_MODEM_GNSS_AGNSS_GPS_EPHEMERIDES                  2
/** @brief GPS/QZSS almanac assistance A-GNSS parameters.
 *
 * @details Data model #nrf_modem_gnss_agnss_gps_data_almanac.
 */
#define NRF_MODEM_GNSS_AGNSS_GPS_ALMANAC                      3
/** @brief Ionospheric assistance A-GNSS parameters, Klobuchar model.
 *
 * @details Data model #nrf_modem_gnss_agnss_data_klobuchar.
 */
#define NRF_MODEM_GNSS_AGNSS_KLOBUCHAR_IONOSPHERIC_CORRECTION 4
/** @brief Ionospheric assistance A-GNSS parameters, NeQuick model.
 *
 * @details Data model #nrf_modem_gnss_agnss_data_nequick.
 */
#define NRF_MODEM_GNSS_AGNSS_NEQUICK_IONOSPHERIC_CORRECTION   5
/** @brief GPS system time and satellite TOW assistance A-GNSS parameters.
 *
 * @details Data model #nrf_modem_gnss_agnss_gps_data_system_time_and_sv_tow.
 */
#define NRF_MODEM_GNSS_AGNSS_GPS_SYSTEM_CLOCK_AND_TOWS        6
/** @brief Location assistance A-GNSS parameters.
 *
 * @details Data model #nrf_modem_gnss_agnss_data_location.
 */
#define NRF_MODEM_GNSS_AGNSS_LOCATION                         7
/** @brief Integrity assistance A-GPS parameters.
 *
 * @details Data model #nrf_modem_gnss_agps_data_integrity.
 */
#define NRF_MODEM_GNSS_AGPS_INTEGRITY                         8
/** @brief Integrity assistance A-GNSS parameters.
 *
 * @details Data model #nrf_modem_gnss_agnss_data_integrity.
 *
 * @note This type is only supported by modem firmware v2.0.0 or later.
 */
#define NRF_MODEM_GNSS_AGNSS_INTEGRITY                        9
/** @} */

/** @defgroup nrf_modem_gnss_event_type Event type enumerator
 *
 * @brief Event types that trigger the handler function.
 *
 * @details The event parameter in the handler function will hold one of these values. Some of the
 *          events have associated payload which can be read using #nrf_modem_gnss_read.
 * @{
 */
/** @brief PVT event.
 *
 * @details Payload is of type @ref nrf_modem_gnss_pvt_data_frame.
 */
#define NRF_MODEM_GNSS_EVT_PVT                 1
/** @brief GNSS fix event.
 *
 * @details Payload is of type @ref nrf_modem_gnss_pvt_data_frame.
 *
 * @note In periodic navigation mode with modem firmware v2.0.0 or later, this event is not sent
 *       when GNSS is running because of a scheduled download.
 */
#define NRF_MODEM_GNSS_EVT_FIX                 2
/** @brief NMEA event.
 *
 * @details Payload is of type @ref nrf_modem_gnss_nmea_data_frame.
 */
#define NRF_MODEM_GNSS_EVT_NMEA                3
/** @brief Need new A-GNSS data event.
 *
 * @details Payload is of type @ref nrf_modem_gnss_agnss_data_frame.
 */
#define NRF_MODEM_GNSS_EVT_AGNSS_REQ           4
/** @brief GNSS is blocked by LTE event.
 *
 * @details This event doesn't have any payload.
 */
#define NRF_MODEM_GNSS_EVT_BLOCKED             5
/** @brief GNSS is unblocked by LTE event.
 *
 * @details This event doesn't have any payload.
 */
#define NRF_MODEM_GNSS_EVT_UNBLOCKED           6
/** @brief GNSS woke up in periodic mode.
 *
 * @details This event is sent when GNSS receiver is turned on in periodic mode. This happens when
 *          GNSS starts acquiring the next periodic fix but also when a scheduled download starts.
 *
 * This event doesn't have any payload.
 */
#define NRF_MODEM_GNSS_EVT_PERIODIC_WAKEUP     7
/** @brief GNSS enters sleep because fix retry timeout was reached in periodic or single fix mode.
 *
 * @details This event doesn't have any payload.
 */
#define NRF_MODEM_GNSS_EVT_SLEEP_AFTER_TIMEOUT 8
/** @brief GNSS enters sleep because fix was achieved in periodic mode.
 *
 * @details This event doesn't have any payload.
 */
#define NRF_MODEM_GNSS_EVT_SLEEP_AFTER_FIX     9
/** @brief Reference altitude for 3-satellite fix expired.
 *
 * @details This event doesn't have any payload.
 */
#define NRF_MODEM_GNSS_EVT_REF_ALT_EXPIRED     10
/** @} */

/** @defgroup nrf_modem_gnss_data_type Data type enumerator
 *
 * @brief Use these values to indicate the type of data read using #nrf_modem_gnss_read.
 * @{
 */
/** @brief PVT (Position-Velocity-Time) data.
 *
 * @details Use this data type to read PVT data using the #nrf_modem_gnss_read function. New PVT
 *          data will be available when an #NRF_MODEM_GNSS_EVT_PVT or #NRF_MODEM_GNSS_EVT_FIX event
 *          is received.
 */
#define NRF_MODEM_GNSS_DATA_PVT       1
/** @brief NMEA data.
 *
 * @details Use this data type to read NMEA data using the #nrf_modem_gnss_read function. New NMEA
 *          data will be available when an #NRF_MODEM_GNSS_EVT_NMEA event is received.
 *
 * @note There's no buffering for NMEA data. Because of this NMEA data needs to be read inside the
 *       event handler function, otherwise some of the data may be overwritten before it's read.
 */
#define NRF_MODEM_GNSS_DATA_NMEA      2
/** @brief A-GNSS request data.
 *
 * @details Use this data type to read A-GNSS request data using the #nrf_modem_gnss_read function.
 *          New A-GNSS request data is received when an #NRF_MODEM_GNSS_EVT_AGNSS_REQ event is
 *          received.
 */
#define NRF_MODEM_GNSS_DATA_AGNSS_REQ 3
/** @} */

/** @brief Configuration for the #nrf_modem_gnss_1pps_enable function. */
struct nrf_modem_gnss_1pps_config {
	/** @brief Pulse interval in seconds. Range 0...1800s. Value 0 denotes one-time pulse mode.
	 */
	uint16_t pulse_interval;
	/** @brief Pulse width in milliseconds. Range 1...500ms. */
	uint16_t pulse_width;
	/** @brief If true, pulses are started at given start time. If false, the date and time
	 *         fields are ignored.
	 */
	bool apply_start_time;
	/** @brief Gregorian year. Range 0...4000. */
	uint16_t year;
	/** @brief Month of the year. Range 1...12. */
	uint8_t month;
	/** @brief Day of the month. Range 1...31. */
	uint8_t day;
	/** @brief Hour of the day. Range 0...23. */
	uint8_t hour;
	/** @brief Minute of the hour. Range 0...59. */
	uint8_t minute;
	/** @brief Second of the minute. Range 0...59. */
	uint8_t second;
};

/** @defgroup nrf_modem_gnss_timing_source Sleep timing source enumerator
 *
 * @brief Used to select which sleep timing source GNSS uses.
 *
 * @details Using TCXO instead of RTC during GNSS sleep periods might be beneficial when used with
 *          1PPS. When GNSS is not running all the time (periodic navigation or duty-cycling is
 *          used), 1PPS accuracy can be improved by using TCXO. It may also improve sensitivity for
 *          periodic navigation when the fix interval is short.
 *
 * @note Use of TCXO significantly raises the idle current consumption.
 * @{
 */
/** @brief Real time clock (RTC). */
#define NRF_MODEM_GNSS_TIMING_SOURCE_RTC  0
/** @brief Temperature compensated crystal oscillator (TCXO). */
#define NRF_MODEM_GNSS_TIMING_SOURCE_TCXO 1
/** @} */

/** @defgroup nrf_modem_gnss_qzss_nmea_mode QZSS NMEA mode enumerator
 *
 * @brief Use these values to select if QZSS satellites are reported in NMEA sentences or not.
 * @{
 */
/** @brief Standard NMEA reporting.
 *
 * @details QZSS satellites not reported in GPGSA and GPGSV sentences.
 */
#define NRF_MODEM_GNSS_QZSS_NMEA_MODE_STANDARD 0
/** @brief Custom NMEA reporting with QZSS satellites.
 *
 * @details QZSS satellites reported in GPGSA and GPGSV sentences using satellite IDs 193...202.
 */
#define NRF_MODEM_GNSS_QZSS_NMEA_MODE_CUSTOM   1
/** @} */

/** @brief Date and time. */
struct nrf_modem_gnss_datetime {
	/** 4-digit representation (Gregorian calendar). */
	uint16_t year;
	/** 1...12 */
	uint8_t month;
	/** 1...31 */
	uint8_t day;
	/** 0...23 */
	uint8_t hour;
	/** 0...59 */
	uint8_t minute;
	/** 0...59 */
	uint8_t seconds;
	/** 0...999 */
	uint16_t ms;
};

/** @brief Space Vehicle (SV) information. */
struct nrf_modem_gnss_sv {
	/** Satellite ID.
	 *  1...32 for GPS
	 *  193...202 for QZSS.
	 */
	uint16_t sv;
	/** Signal type, see @ref nrf_modem_gnss_signal_id. */
	uint8_t signal;
	/** 0.1 dB/Hz. */
	uint16_t cn0;
	/** Satellite elevation angle in degrees. */
	int16_t elevation;
	/** Satellite azimuth angle in degrees. */
	int16_t azimuth;
	/** See @ref nrf_modem_gnss_sv_flag_bitmask. */
	uint8_t flags;
};

/** @brief Position, Velocity and Time (PVT) data frame. */
struct nrf_modem_gnss_pvt_data_frame {
	/** Latitude in degrees. */
	double latitude;
	/** Longitude in degrees. */
	double longitude;
	/** Altitude above WGS-84 ellipsoid in meters. */
	float altitude;
	/** Position accuracy (2D 1-sigma) in meters. */
	float accuracy;
	/** Altitude accuracy (1-sigma) in meters. */
	float altitude_accuracy;
	/** Horizontal speed in m/s. */
	float speed;
	/** Speed accuracy (1-sigma) in m/s. */
	float speed_accuracy;
	/** Vertical speed in m/s. Positive is up. */
	float vertical_speed;
	/** Vertical speed accuracy (1-sigma) in m/s. */
	float vertical_speed_accuracy;
	/** Heading of user movement in degrees. */
	float heading;
	/** Heading accuracy (1-sigma) in degrees. */
	float heading_accuracy;
	/** Date and time. */
	struct nrf_modem_gnss_datetime datetime;
	/** Position dilution of precision. */
	float pdop;
	/** Horizontal dilution of precision. */
	float hdop;
	/** Vertical dilution of precision. */
	float vdop;
	/** Time dilution of precision. */
	float tdop;
	/** See @ref nrf_modem_gnss_pvt_flag_bitmask. */
	uint8_t flags;
	/** Describes up to 12 of the satellites used for the measurement. */
	struct nrf_modem_gnss_sv sv[NRF_MODEM_GNSS_MAX_SATELLITES];
	/** Cumulative GNSS execution time since last start in milliseconds. */
	uint32_t execution_time;
};

/** @brief Maximum length of an NMEA string including the null-terminator. */
#define NRF_MODEM_GNSS_NMEA_MAX_LEN 83

/** @brief Single null-terminated NMEA sentence. */
struct nrf_modem_gnss_nmea_data_frame {
	/** Null-terminated NMEA sentence. */
	char nmea_str[NRF_MODEM_GNSS_NMEA_MAX_LEN];
};

/** @defgroup nrf_modem_gnss_agnss_data_bitmask A-GNSS data request bitmask values
 *
 * @brief Bitmask values for the data_flags member in the #nrf_modem_gnss_agnss_data_frame.
 * @{
 */
/** @brief GPS UTC parameters assistance data. */
#define NRF_MODEM_GNSS_AGNSS_GPS_UTC_REQUEST			0x01
/** @brief Klobuchar ionospheric correction parameters assistance data. */
#define NRF_MODEM_GNSS_AGNSS_KLOBUCHAR_REQUEST			0x02
/** @brief NeQuick ionospheric correction parameters assistance data. */
#define NRF_MODEM_GNSS_AGNSS_NEQUICK_REQUEST			0x04
/** @brief GPS system time and SV TOWs assistance data. */
#define NRF_MODEM_GNSS_AGNSS_GPS_SYS_TIME_AND_SV_TOW_REQUEST	0x08
/** @brief Position assistance parameters assistance data */
#define NRF_MODEM_GNSS_AGNSS_POSITION_REQUEST			0x10
/** @brief Integrity assistance parameters assistance data */
#define NRF_MODEM_GNSS_AGNSS_INTEGRITY_REQUEST			0x20
/** @} */

/** @brief Ephemeris and almanac data need for a system. */
struct nrf_modem_gnss_agnss_system_data_need {
	/** @brief System ID, see @ref nrf_modem_gnss_system_id. */
	uint8_t system_id;
	/** @brief Bit mask indicating the satellites for which the assistance ephemeris data
	 *         is needed.
	 */
	uint64_t sv_mask_ephe;
	/** @brief Bit mask indicating the satellites for which the assistance almanac data
	 *         is needed.
	 */
	uint64_t sv_mask_alm;
};

/** @brief A-GNSS notification data frame used by GNSS to let the application know it needs new
 *         assistance data.
 */
struct nrf_modem_gnss_agnss_data_frame {
	/** @brief Indicating what kind of A-GNSS data is needed by GNSS.
	 *
	 * @details See @ref nrf_modem_gnss_agnss_data_bitmask.
	 */
	uint32_t data_flags;
	/** @brief Number of systems assistance data need is indicated for. */
	uint8_t system_count;
	/** @brief Ephemeris and almanac data need for each system. */
	struct nrf_modem_gnss_agnss_system_data_need system[NRF_MODEM_GNSS_MAX_SYSTEMS];
};

/** The maximum number of satellites expiry times are given for. */
#define NRF_MODEM_GNSS_MAX_SV_EXPIRY_TIMES 127

/** @brief Ephemeris and almanac expiry times for a satellite. */
struct nrf_modem_gnss_agnss_sv_expiry {
	/** @brief Satellite ID. */
	uint8_t sv_id;
	/** @brief System ID, see @ref nrf_modem_gnss_system_id. */
	uint8_t system_id;
	/** @brief Ephemeris expiry time in minutes. */
	uint16_t ephe_expiry;
	/** @brief Almanac expiry time in minutes. */
	uint16_t alm_expiry;
};

/** @brief A-GNSS data expiry.
 *
 * @details Expiry times are given in minutes. 0 indicates that data is needed immediately.
 *          0xffff indicates that data is not used by GNSS.
 */
struct nrf_modem_gnss_agnss_expiry {
	/** @brief Flags indicating the A-GNSS data types needed by GNSS.
	 *
	 * @details See @ref nrf_modem_gnss_agnss_data_bitmask. If bit
	 *          @ref NRF_MODEM_GNSS_AGNSS_GPS_SYS_TIME_AND_SV_TOW_REQUEST is set,
	 *          GNSS doesn't have the GPS system time and all expiry time fields
	 *          for ephemerides and almanacs are set to zero.
	 */
	uint32_t data_flags;
	/** @brief UTC parameters expiry time. */
	uint16_t utc_expiry;
	/** @brief Klobuchar ionospheric correction parameters expiry time. */
	uint16_t klob_expiry;
	/** @brief NeQuick ionospheric correction parameters expiry time. */
	uint16_t neq_expiry;
	/** @brief Integrity assistance parameters expiry time. */
	uint16_t integrity_expiry;
	/** @brief Position assistance expiry time. */
	uint16_t position_expiry;
	/** @brief Number of satellites for which expiry times are given. */
	uint8_t sv_count;
	/** @brief Ephemeris and almanac expiry times for satellites. */
	struct nrf_modem_gnss_agnss_sv_expiry sv[NRF_MODEM_GNSS_MAX_SV_EXPIRY_TIMES];
};

/** @brief A-GNSS GPS UTC parameters. */
struct nrf_modem_gnss_agnss_gps_data_utc {
	/** First order term of polynomial (sec/sec). Scale factor 2^-50.
	 *  Range -8388608...8388607 (25 bits).
	 */
	int32_t a1;
	/** Constant term of polynomial (sec). Scale factor 2^-30. */
	int32_t a0;
	/** UTC reference GPS time-of-week (sec). Scale factor 2^12. Range 0..147. */
	uint8_t tot;
	/** UTC reference GPS week number modulo 256. */
	uint8_t wn_t;
	/** Current or past leap second count (sec). */
	int8_t delta_tls;
	/** Leap second reference GPS week number modulo 256. */
	uint8_t wn_lsf;
	/** Leap second reference GPS day-of-week (day). Range 1...7. */
	int8_t dn;
	/** Current or future leap second count (sec) (total size
	 *  of the type-specific assistance data).
	 */
	int8_t delta_tlsf;
};

/** @brief A-GNSS GPS/QZSS ephemeris data. */
struct nrf_modem_gnss_agnss_gps_data_ephemeris {
	/** Satellite ID (dimensionless).
	 *  1...32: GPS
	 *  193...202: QZSS
	 */
	uint8_t sv_id;
	/** Satellite health (dimensionless). */
	uint8_t health;
	/** Issue of data, clock parameters (dimensionless). Range 0...2047 (11 bits). */
	uint16_t iodc;
	/** Clock parameters reference GPS time-of-week (sec). Scale factor 2^4. Range 0...37799. */
	uint16_t toc;
	/** Clock drift rate (sec/sec2). Scale factor 2^-55. */
	int8_t af2;
	/** Clock drift (sec/sec). Scale factor 2^-43. */
	int16_t af1;
	/** Clock bias (sec). Scale factor 2^-31. Range -2097152...2097151 (22 bit) */
	int32_t af0;
	/** Group delay (sec). Scale factor 2^-31. */
	int8_t tgd;
	/** URA index (dimensionless). Range 0...15. */
	uint8_t ura;
	/** Curve fit interval indication. Range 0...1. */
	uint8_t fit_int;
	/** Ephemeris parameters reference GPS time-of-week (sec).
	 *  Scale factor 2^4. Range 0...37799.
	 */
	uint16_t toe;
	/** Argument of perigee (semi-circle). Scale factor 2^-31. */
	int32_t w;
	/** Mean motion difference (semi-circle/sec). Scale factor 2^-43. */
	int16_t delta_n;
	/** Mean anomaly at reference time (semi-circle). Scale factor 2^-31. */
	int32_t m0;
	/** Rate of right ascension (semi-circle/sec). Scale factor 2^-43.
	 *  Range -8388608...8388607 (24 bits).
	 */
	int32_t omega_dot;
	/** Eccentricity (dimensionless). Scale factor 2^-33. */
	uint32_t e;
	/** Rate of inclination angle (semi-circle/sec). Scale factor 2-43.
	 *  Range -8192...8191 (14 bits).
	 */
	int16_t idot;
	/** Square root of semi-major axis (m). Scale factor 2^-19. */
	uint32_t sqrt_a;
	/** Inclination angle at reference time (semi-circle). Scale factor 2^-31. */
	int32_t i0;
	/** Longitude of ascending node at weekly epoch (semi-circle). Scale factor 2^-31. */
	int32_t omega0;
	/** Orbit radius, sine harmonic amplitude (m). Scale factor 2^-5. */
	int16_t crs;
	/** Inclination angle, sine harmonic amplitude (rad). Scale factor 2^-29. */
	int16_t cis;
	/** Argument of latitude, sine harmonic amplitude (rad). Scale factor 2^-29. */
	int16_t cus;
	/** Orbit radius, cosine harmonic amplitude (m). Scale factor 2^-5. */
	int16_t crc;
	/** Inclination angle, cosine harmonic amplitude (rad). Scale factor 2^-29. */
	int16_t cic;
	/** Argument of latitude, cosine harmonic amplitude (rad). Scale factor 2^-29. */
	int16_t cuc;
};

/** @brief A-GNSS GPS/QZSS almanac data. */
struct nrf_modem_gnss_agnss_gps_data_almanac {
	/** Satellite ID (dimensionless).
	 *  1...32: GPS
	 *  193...202: QZSS
	 */
	uint8_t sv_id;
	/** Almanac reference GPS week number modulo 256. */
	uint8_t wn;
	/** Almanac reference GPS time-of-week (sec). Scale factor 2^12. Range 0...147. */
	uint8_t toa;
	/** Issue of data, almanac (dimensionless). Range 0...3  (2 bits). */
	uint8_t ioda;
	/** Eccentricity (dimensionless). Scale factor 2^-21. */
	uint16_t e;
	/** Correction to inclination (semi-circle). Scale factor 2^-19. */
	int16_t delta_i;
	/** Rate of right ascension (semi-circle/sec). Scale factor 2^-38. */
	int16_t omega_dot;
	/** Satellite health (dimensionless) */
	uint8_t sv_health;
	/** Square root of semi-major axis (m^(1/2)).
	 *  Scale factor 2^-11. Range 0...16777215 (24 bit).
	 */
	uint32_t sqrt_a;
	/** Longitude of ascending node at weekly epoch (semi-circle).
	 *  Scale factor 2^-23. Range -8388608...8388607  (24 bits).
	 */
	int32_t omega0;
	/** Argument of perigee (semi-circle). Scale factor 2^-23. */
	int32_t w;
	/** Mean anomaly at reference time (semi-circle).
	 *  Scale factor 2^-23. Range -8388608...8388608 (24 bits).
	 */
	int32_t m0;
	/** Clock bias (sec). Scale factor 2^-20. Range -1024...1023 (11 bits). */
	int16_t af0;
	/** Clock drift (sec/sec). Scale factor 2^-38. Range -1024...1023  (11 bits). */
	int16_t af1;
};

/** @brief A-GNSS Klobuchar ionospheric correction data. */
struct nrf_modem_gnss_agnss_data_klobuchar {
	/** Constant term (sec). Scale factor 2^-30. */
	int8_t alpha0;
	/** First-order coefficient (sec/semi-circle). Scale factor 2^-27. */
	int8_t alpha1;
	/** Second-order coefficient (sec/semi-circle^2). Scale factor 2^-24. */
	int8_t alpha2;
	/** Third-order coefficient (sec/semi-circle^3). Scale factor 2^-24. */
	int8_t alpha3;
	/** Constant term (sec). Scale factor 2^11. */
	int8_t beta0;
	/** First-order coefficient (sec/semi-circle). Scale factor 2^14. */
	int8_t beta1;
	/** Second-order coefficient (sec/semi-circle^2). Scale factor 2^16. */
	int8_t beta2;
	/** Third-order coefficient (sec/semi-circle^3). Scale factor 2^16. */
	int8_t beta3;
};

/** @brief A-GNSS NeQuick ionospheric correction data. */
struct nrf_modem_gnss_agnss_data_nequick {
	/** Effective ionisation level 1st order parameter (SFU).
	 * Scale factor 2^-2. Range 0...2047  (11 bits).
	 */
	int16_t ai0;
	/** Effective ionisation level 2nd order parameter (SFU/deg). Scale factor 2^-8.
	 *  Range -1024...1023 (11 bits).
	 */
	int16_t ai1;
	/** Effective ionisation level 3rd order parameter (SFU/deg^2). Scale factor 2^-15.
	 *  Range -8192...8191  (14 bits).
	 */
	int16_t ai2;
	/** Storm condition bit mask indicating the ionospheric
	 *  storm condition for different regions.
	 */
	uint8_t storm_cond;
	/** Storm validity bit mask indicating for which regions
	 *  the ionospheric storm condition bit is valid.
	 */
	uint8_t storm_valid;
};

/** @brief A-GNSS GPS TOW assistance data for a satellite. */
struct nrf_modem_gnss_agnss_gps_data_tow_element {
	/** First two bits (MSB) represent the reserved bit and integrity status flag
	 *  in the telemetry message (TLM) word. The following 14 bits represent the TLM
	 *  being broadcast by the satellite.
	 */
	uint16_t tlm;
	/** Bit 0 (LSB): anti-spoof flag. Bit 1: alert flag. */
	uint8_t flags;
};

/** @brief Maximum number of TOW elements in the TOW array.
 *
 * @see nrf_modem_gnss_agnss_gps_data_system_time_and_sv_tow.
 */
#define NRF_MODEM_GNSS_AGNSS_GPS_MAX_SV_TOW 32

/** @brief A-GNSS GPS system time and satellite TOW data. */
struct nrf_modem_gnss_agnss_gps_data_system_time_and_sv_tow {
	/** Day number since Jan 6th, 1980 00:00:00 UTC (USNO). */
	uint16_t date_day;
	/** Full seconds part of time-of-day (s). Range 0...86399. */
	uint32_t time_full_s;
	/** Fraction of a second part of time-of-day (ms). Range 0...999. */
	uint16_t time_frac_ms;
	/** Bit mask indicating the satellite PRNs for which the satellite-specific TOW
	 *  assistance data is valid.
	 */
	uint32_t sv_mask;
	/** TOW assistance data for satellites. */
	struct nrf_modem_gnss_agnss_gps_data_tow_element
		sv_tow[NRF_MODEM_GNSS_AGNSS_GPS_MAX_SV_TOW];
};

/** @brief A-GNSS location data. */
struct nrf_modem_gnss_agnss_data_location {
	/** Geodetic latitude in WGS-84. Range -8388607...8388607.
	 *  The relation between the coded number N and the latitude
	 *  range X (in degrees) is as follows: N <= (2^23/90) * X < N + 1.
	 *  For N = 2^23 - 1, the range is extended to include N+1.
	 *  Range of X (in degrees) -90...90.
	 */
	int32_t latitude;

	/** Geodetic longitude in WGS-84. Range -8388607..8388607.
	 *  The relation between the coded number N and the longitude range
	 *  X (in degrees) is as follows: N <= (2^24/360) * X < N + 1.
	 *  Range of X (in degrees) -180...180.
	 */
	int32_t longitude;

	/** Altitude. Above (positive value) or below (negative value) WGS-84
	 *  ellipsoid surface. Range -32767...32767.
	 *  The relation between the coded number N and the altitude range a
	 *  (in meters) is as follows: N <= a < N + 1.
	 *  For N = 2^15 - 1 the range is extended to include all greater values of a.
	 */
	int16_t altitude;

	/** Uncertainty, semi-major. Range 0...127 or 255 for missing latitude and longitude.
	 *  The uncertainty (in meters) is mapped from the coded number K with following formula:
	 *  r = C * ((1 + x)^K - 1), where C = 10 and x = 0,1. Range of r (in kilometers) 0...1800.
	 */
	uint8_t unc_semimajor;

	/** Uncertainty, semi-minor. Range 0...127 or 255 for missing latitude and longitude.
	 *  The uncertainty (in meters) is mapped from the coded number K with following formula:
	 *  r = C * ((1 + x)^K - 1), where C = 10 and x = 0,1. Range of r (in kilometers) 0...1800.
	 */
	uint8_t unc_semiminor;

	/** Orientation angle between the major axis and north. Range in degrees 0...179. */
	uint8_t orientation_major;

	/** Uncertainty, altitude. Range 0...127 or 255 for missing altitude. The uncertainty in
	 *  altitude h (in meters) is mapped from the coded number K with following formula:
	 *  h = C * ((1 + x)^K - 1), where C = 45 and x = 0,025. Range of h (in meters) 0...990,5.
	 */
	uint8_t unc_altitude;

	/** The confidence level (expressed as a percentage) with which
	 *  the position of a target entity is included within the uncertainty ellipsoid.
	 *  Range 0...128. '0' indicates 'no information'.
	 *  Values 101..128  should be treated as '0'.
	 */
	uint8_t confidence;
};

/** @brief A-GPS satellite integrity data. */
struct nrf_modem_gnss_agps_data_integrity {
	/** Bit mask indicating the unhealthy GPS satellite PRNs. When a mask bit is set,
	 *  the corresponding GPS satellite PRN is unhealthy.
	 */
	uint32_t integrity_mask;
};

/** @brief A-GNSS satellite integrity mask for a signal. */
struct nrf_modem_gnss_agnss_data_signal_integrity {
	/** Signal ID, see @ref nrf_modem_gnss_signal_id. */
	uint8_t signal_id;
	/** Bit mask indicating the unhealthy satellite numbers for the signal. When a mask bit is
	 *  set, the corresponding satellite is unhealthy.
	 *  GPS: 32 LSBs correspond to PRN 1 .. 32
	 *  QZSS: 10 LSBs correspond to PRN 193 .. 202
	 */
	uint64_t integrity_mask;
};

/** @brief A-GNSS satellite integrity data. */
struct nrf_modem_gnss_agnss_data_integrity {
	/** Number of signals integrity is indicated for. */
	uint8_t signal_count;
	/** Satellite integrity mask for each signal. */
	struct nrf_modem_gnss_agnss_data_signal_integrity signal[NRF_MODEM_GNSS_MAX_SIGNALS];
};

/** @defgroup nrf_modem_gnss_delete_bitmask Delete bitmask values
 *
 * @brief Bitmask values to be used as an argument for function #nrf_modem_gnss_nv_data_delete.
 * @{
 */
/** @brief Ephemerides. */
#define NRF_MODEM_GNSS_DELETE_EPHEMERIDES	   0x001
/** @brief Almanacs (excluding leap second and ionospheric correction). */
#define NRF_MODEM_GNSS_DELETE_ALMANACS		   0x002
/** @brief Ionospheric correction parameters. */
#define NRF_MODEM_GNSS_DELETE_IONO_CORRECTION_DATA 0x004
/** @brief Last good fix (the last position). */
#define NRF_MODEM_GNSS_DELETE_LAST_GOOD_FIX	   0x008
/** @brief GPS time-of-week (TOW). */
#define NRF_MODEM_GNSS_DELETE_GPS_TOW		   0x010
/** @brief GPS week number. */
#define NRF_MODEM_GNSS_DELETE_GPS_WEEK		   0x020
/** @brief Leap second (UTC parameters). */
#define NRF_MODEM_GNSS_DELETE_UTC_DATA		   0x040
/** @brief Local clock (TCXO) frequency offset. */
#define NRF_MODEM_GNSS_DELETE_TCXO_OFFSET	   0x080
/** @brief Precision estimate of GPS time-of-week (TOW). */
#define NRF_MODEM_GNSS_DELETE_GPS_TOW_PRECISION	   0x100
/** @brief Extended Kalman Filter (EKF) state (last position/velocity/time solution).
 *
 * @note This value is only supported by modem firmware v2.0.1 or later.
 */
#define NRF_MODEM_GNSS_DELETE_EKF		   0x400
/** @} */

/** @defgroup nrf_modem_gnss_dynamics_mode Dynamics mode enumerator
 *
 * @brief Dynamics mode values to be used as an argument for function
 *        #nrf_modem_gnss_dyn_mode_change.
 *
 * The maximum speeds listed below are for modem firmware v2.0.0 or later. For older modem
 * firmware versions, the assumed maximum speed for all modes is 100 km/h.
 *
 * The maximum speed is not a hard limit, but if the receiver speed is higher than the assumed
 * maximum speed for the selected mode, GNSS will operate with suboptimal predictions.
 *
 * @{
 */
/** @brief General purpose mode. Assumed maximum speed 100 km/h. */
#define NRF_MODEM_GNSS_DYNAMICS_GENERAL_PURPOSE 0
/** @brief Optimize for stationary use case. Assumed maximum speed 5 km/h. */
#define NRF_MODEM_GNSS_DYNAMICS_STATIONARY      1
/** @brief Optimize for pedestrian use case. Assumed maximum speed 30 km/h. */
#define NRF_MODEM_GNSS_DYNAMICS_PEDESTRIAN      2
/** @brief Optimize for automotive use case. Assumed maximum speed > 100 km/h. */
#define NRF_MODEM_GNSS_DYNAMICS_AUTOMOTIVE      3
/** @} */

/** @brief Event handler prototype.
 *
 * @param[in] event Event ID, see @ref nrf_modem_gnss_event_type.
 */
typedef void (*nrf_modem_gnss_event_handler_type_t)(int event);

/** @brief Sets the GNSS event handler.
 *
 * @details This function is used to set the function that handles events from GNSS.
 *
 * @note The event handler is executed in interrupt context so a lot of processing inside the
 *       handler may have performance implications and side effects. It's recommended to use the
 *       handler only for signaling or re-scheduling processing to a separate thread. Data can be
 *       read at any time while GNSS is running, the events are only signaling that the data has
 *       been updated.
 *
 * @param[in] handler Pointer to the event handler function or NULL.
 *
 * @retval 0 on success.
 */
int32_t nrf_modem_gnss_event_handler_set(nrf_modem_gnss_event_handler_type_t handler);

/** @brief Sets the supported GNSS signals. Signals which have the corresponding bit set are
 *         enabled.
 *
 * Default value: All signals supported by the modem firmware are enabled
 *
 * @note GPS L1 C/A can't be disabled and remains enabled even if the corresponding bit is not set.
 *
 * @param[in] signal_mask Signal bitmask, see @ref nrf_modem_gnss_signal_bitmask.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is running.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_signal_mask_set(uint8_t signal_mask);

/** @brief Set below which elevation angle GNSS should stop tracking a satellite.
 *
 * @details Satellites with elevation angle less than the threshold are excluded from the
 *          estimation.
 *
 * Default value: 5 deg
 *
 * @param[in] angle Value between 0 and 90 (degrees above the horizon).
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is running.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_elevation_threshold_set(uint8_t angle);

/** @brief Sets targeted start and runtime performance.
 *
 * @details Default value: #NRF_MODEM_GNSS_USE_CASE_MULTIPLE_HOT_START
 *
 * @param[in] use_case Bit mask, see @ref nrf_modem_gnss_use_case_bitmask.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is running.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_use_case_set(uint8_t use_case);

/** @brief Sets the GNSS fix interval in seconds.
 *
 * @details Single fix navigation mode is engaged by setting the fix interval to 0.
 *
 * Continuous navigation mode is engaged by setting fix interval to 1.
 *
 * Periodic navigation mode is engaged by setting the fix interval to value 10...65535. The unit is
 * seconds.
 *
 * Default value: 1 (continuous navigation)
 *
 * @note In periodic navigation mode, when GNSS determines it needs to download ephemerides or
 *       almanacs from the broadcast, the fix interval and fix retry parameters are temporarily
 *       ignored. GNSS will perform scheduled downloads until it has downloaded the data it needs,
 *       after which normal operation is resumed.
 *
 * @param[in] fix_interval 0 for single fix, 1 for continuous navigation or fix interval in seconds.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is running.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_fix_interval_set(uint16_t fix_interval);

/** @brief Sets the GNSS fix retry period in seconds.
 *
 * @details Fix retry parameter controls the maximum time the GNSS receiver is allowed to run while
 *          trying to produce a valid PVT estimate. If the fix retry time is non-zero, the GNSS
 *          receiver is turned off after the fix retry time is up regardless of whether a valid PVT
 *          estimate was produced or not. If fix retry parameter is set to zero, the GNSS receiver
 *          is allowed to run indefinitely until a valid PVT estimate is produced.
 *
 * Default value: 60s
 *
 * @note For the first fix in periodic navigation mode the GNSS receiver is allowed to run for 60
 *       seconds even if the retry interval is set shorter.
 *
 * @note Fix retry has no effect in continuous navigation mode.
 *
 * @param[in] fix_retry Fix retry period in seconds.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is running.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_fix_retry_set(uint16_t fix_retry);

/** @brief Sets which NMEA strings are enabled.
 *
 * @details Default value: 0
 *
 * @param[in] nmea_mask Bit mask, see @ref nrf_modem_gnss_nmea_string_bitmask.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is running.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_nmea_mask_set(uint16_t nmea_mask);

/** @brief Sets the used power saving mode.
 *
 * @details Valid only in continuous navigation mode.
 *
 * Default value: #NRF_MODEM_GNSS_PSM_DISABLED
 *
 * @param[in] power_mode See @ref nrf_modem_gnss_power_save_modes.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is running.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_power_mode_set(uint8_t power_mode);

/** @brief Sets the used timing source during GNSS sleep.
 *
 * @details Using TCXO instead of RTC during GNSS sleep periods might be beneficial when used with
 *          1PPS. When GNSS is not running all the time (periodic navigation or duty cycling is
 *          used), 1PPS accuracy can be improved by using TCXO. It may also improve sensitivity for
 *          periodic navigation when the fix interval is short.
 *
 * Default value: #NRF_MODEM_GNSS_TIMING_SOURCE_RTC
 *
 * @note Use of TCXO significantly raises the idle current consumption.
 *
 * @param[in] timing_source See @ref nrf_modem_gnss_timing_source.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is running.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_timing_source_set(uint8_t timing_source);

/** @brief Sets the used QZSS NMEA mode.
 *
 * @details NMEA 4.10 standard does not support QZSS satellites, so in standard NMEA mode QZSS
 *          satellites are not reported in GPGSA and GPGSV sentences. In custom NMEA mode satellite
 *          IDs 193...202 are used for QZSS satellites.
 *
 * Default value: #NRF_MODEM_GNSS_QZSS_NMEA_MODE_STANDARD
 *
 * @param[in] nmea_mode See @ref nrf_modem_gnss_qzss_nmea_mode.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is running.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_qzss_nmea_mode_set(uint8_t nmea_mode);

/** @brief Sets which QZSS PRNs are enabled.
 *
 * @details QZSS satellite acquisition and tracking can be configured per satellite using QZSS PRN
 *          mask. Bits 0...9 correspond to QZSS PRNs 193...202, respectively. When a bit is set,
 *          usage of the corresponding QZSS satellite is enabled. Bits 10...15 are reserved and
 *          their value is ignored.
 *
 * Default value: All QZSS PRNs (193...202) are enabled.
 *
 * @param[in] prn_mask QZSS PRN mask.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is running.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_qzss_prn_mask_set(uint16_t prn_mask);

/** @brief Starts GNSS.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_start(void);

/** @brief Stops GNSS.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_stop(void);

/** @brief Deletes GNSS data from non-volatile memory.
 *
 * @details This function is used to delete downloaded data objects stored in non-volatile memory.
 *          The GNSS module stores this between sessions to get a fix faster on the next startup.
 *          This can for instance be used to trigger cold or warm starts.
 *
 * @note This is considered a debug feature, and is not supposed to be used in production code.
 *
 * @param[in] delete_mask Objects to be deleted as a bitmask, see
 *                        @ref nrf_modem_gnss_delete_bitmask.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is running.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_nv_data_delete(uint32_t delete_mask);

/** @brief Enables GNSS priority over LTE idle mode procedures.
 *
 * @details GNSS can produce a cold start fix when there are long enough gaps in LTE idle mode
 *          operations. LTE idle eDRX cycles approximately 1 min and above will usually guarantee
 *          the ability to produce a fix. Shorter DRX cycles will be enough after GNSS has got
 *          ephemerides. GNSS priority can be enabled to help getting GNSS into that state, if there
 *          are not long enough gaps in LTE idle mode functionality. Priority for GNSS should be
 *          used only when fix has been blocked by LTE idle mode operations, which can be detected
 *          by #NRF_MODEM_GNSS_PVT_FLAG_NOT_ENOUGH_WINDOW_TIME bit being set in the PVT data frame
 *          flags member. The application should not make the decision based on a single PVT event,
 *          but should enable priority only in case this flag has been set in several consecutive
 *          PVT events. Priority will be disabled automatically after producing the first fix.
 *
 * @note GNSS priority may interfere with LTE operations, if possible it would be good to time the
 *       use of priority to moments where data transfer is not anticipated. In general, long enough
 *       eDRX cycles, or PSM, will ensure better functionality for both GNSS and LTE.
 *
 * @note Priority will not stop LTE connections.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is stopped.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_prio_mode_enable(void);

/** @brief Disables GNSS priority over LTE idle mode procedures.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is stopped.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_prio_mode_disable(void);

/** @brief Changes GNSS dynamics mode.
 *
 * @details Dynamics mode can be used to tune GNSS performance for a specific use case. Use of a
 *          matching dynamics mode improves the positioning performance. Dynamics mode can be
 *          changed without disruption in positioning. The selected dynamics mode is stored into
 *          non-volatile memory.
 *
 * Default value: #NRF_MODEM_GNSS_DYNAMICS_GENERAL_PURPOSE
 *
 * @param[in] mode Selected dynamics mode, see @ref nrf_modem_gnss_dynamics_mode.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is stopped.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_dyn_mode_change(uint32_t mode);

/** @brief Enables 1PPS functionality.
 *
 * @details #nrf_modem_gnss_1pps_config contains the used 1PPS configuration. GNSS only starts
 *          giving pulses after it has got at least one fix. After this, the pulses will continue
 *          also when GNSS is no longer running, but the precision will start degrading.
 *
 * @note The 1PPS feature must not be used when LTE is enabled.
 *
 * @param[in] config Pointer to the configuration struct.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The config pointer is NULL, GNSS returned an error or
 *                     the operation cannot be executed while GNSS is running.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_1pps_enable(const struct nrf_modem_gnss_1pps_config *config);

/** @brief Disables 1PPS functionality.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The GNSS stack returned an error or
 *                     the operation cannot be executed while GNSS is running.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_1pps_disable(void);

/** @brief Reads data from GNSS.
 *
 * @details This function is used to read any data from GNSS. The data type is specified using the
 *          type argument to this function.
 *
 * @param[out] buf Pointer to a buffer that can hold the selected data type.
 * @param[in] buf_len Length of the buffer.
 * @param[in] type The data type to read, see @ref nrf_modem_gnss_data_type.
 *
 * @retval 0 on success.
 * @retval -NRF_EINVAL The selected data type don't exist or supplied buffer is NULL.
 * @retval -NRF_EMSGSIZE The buffer supplied by the application is too small for the selected
 *                       data type.
 * @retval -NRF_ENOMSG There is no data to read for the selected data type.
 */
int32_t nrf_modem_gnss_read(void *buf, int32_t buf_len, int type);

/** @brief Writes A-GNSS data to GNSS.
 *
 * @details This function is used to write A-GNSS data to GNSS to decrease the time to first fix.
 *
 * @param[in] buf Pointer to a buffer that contains A-GNSS data.
 * @param[in] buf_len Length of the buffer.
 * @param[in] type A-GNSS data type identifier, see @ref nrf_modem_gnss_agnss_data_type.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The buffer supplied by the application is NULL, buffer length is 0,
 *                     the A-GNSS data type doesn't exist or the A-GNSS data was not accepted by
 *                     GNSS.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_agnss_write(void *buf, int32_t buf_len, uint16_t type);

/** @brief Reads the current A-GNSS data need and expiry times from GNSS.
 *
 * @details Returns detailed information about the current GNSS A-GNSS data need. The data
 *          structure contains expiry times in seconds for different types of assistance data.
 *          This function can be called at any time. If GNSS doesn't know the current GPS
 *          system time, the expiry times can not be calculated
 *
 * @note When A-GNSS data has been injected but GNSS has not yet been started, ephemerides and
 *       almanacs are in some cases reported as expired. This happens when ephemerides and almanacs
 *       are injected before GPS system time. This can be prevented by either injecting GPS system
 *       time before ephemerides and almanacs or briefly starting and stopping GNSS before calling
 *       the function.
 *
 * @param[out] agnss_expiry Pointer to a buffer where the data is stored to.
 *
 * @return 0 on success.
 * @retval -NRF_EPERM The Modem library is not initialized.
 * @retval -NRF_EACCES GNSS is not enabled in system or functional mode.
 * @retval -NRF_EINVAL The buffer supplied by the application is NULL or
 *                     The GNSS stack returned an error.
 * @retval -NRF_ENOMEM There is not enough shared memory for this request.
 * @retval -NRF_EOPNOTSUPP The operation is not supported by the modem firmware.
 * @retval -NRF_ESHUTDOWN The modem was shut down.
 */
int32_t nrf_modem_gnss_agnss_expiry_get(struct nrf_modem_gnss_agnss_expiry *agnss_expiry);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_GNSS_H__ */
/** @} */
