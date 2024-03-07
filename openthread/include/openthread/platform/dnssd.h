/*
 *  Copyright (c) 2023, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @brief
 *   This file includes the platform abstraction for DNS-SD (e.g., mDNS) on the infrastructure network.
 *
 */

#ifndef OPENTHREAD_PLATFORM_DNSSD_H_
#define OPENTHREAD_PLATFORM_DNSSD_H_

#include <stdint.h>

#include <openthread/dns.h>
#include <openthread/error.h>
#include <openthread/instance.h>
#include <openthread/ip6.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup plat-dns-sd
 *
 * @brief
 *   This module includes the platform abstraction for DNS-SD (e.g., mDNS) on the infrastructure network.
 *
 * @{
 *
 * The DNS-SD platform APIs are used only when `OPENTHREAD_CONFIG_PLATFORM_DNSSD_ENABLE` is enabled.
 *
 */

/**
 * Represents the state of the DNS-SD platform.
 *
 */
typedef enum otPlatDnssdState
{
    OT_PLAT_DNSSD_STOPPED, ///< Stopped and unable to register any service or host.
    OT_PLAT_DNSSD_READY,   ///< Running and ready to register service or host.
} otPlatDnssdState;

/**
 * Represents a request ID for registering/unregistering a service or host.
 *
 */
typedef uint32_t otPlatDnssdRequestId;

/**
 * Represents the callback function used when registering/unregistering a host or service.
 *
 * See `otPlatDnssdRegisterService()`, `otPlatDnssdUnregisterService()`, `otPlatDnssdRegisterHost()`, and
 * `otPlatDnssdUnregisterHost()` for more details about when to invoke the callback and the `aError` values that can
 * be returned in each case.
 *
 * @param[in] aInstance     The OpenThread instance.
 * @param[in] aRequestId    The request ID.
 * @param[in] aError        Error indicating the outcome of request.
 *
 */
typedef void (*otPlatDnssdRegisterCallback)(otInstance *aInstance, otPlatDnssdRequestId aRequestId, otError aError);

/**
 * Represents a DNS-SD service.
 *
 * See `otPlatDnssdRegisterService()`, `otPlatDnssdUnregisterService()` for more details about fields in each case.
 *
 */
typedef struct otPlatDnssdService
{
    const char        *mHostName;            ///< The host name (does not include domain name).
    const char        *mServiceInstance;     ///< The service instance name label (not the full name).
    const char        *mServiceType;         ///< The service type (e.g., "_mt._udp", does not include domain name).
    const char *const *mSubTypeLabels;       ///< Array of sub-type labels (can be NULL if no label).
    uint16_t           mSubTypeLabelsLength; ///< Length of array of sub-type labels.
    const uint8_t     *mTxtData;             ///< Encoded TXT data bytes.
    uint16_t           mTxtDataLength;       ///< Length of TXT data.
    uint16_t           mPort;                ///< The service port number.
    uint16_t           mPriority;            ///< The service priority.
    uint16_t           mWeight;              ///< The service weight.
    uint32_t           mTtl;                 ///< The service TTL in seconds.
    uint32_t           mInfraIfIndex;        ///< The infrastructure network interface index.
} otPlatDnssdService;

/**
 * Represents a DNS-SD host.
 *
 * See `otPlatDnssdRegisterHost()`, `otPlatDnssdUnregisterHost()` for more details about fields in each case.
 *
 */
typedef struct otPlatDnssdHost
{
    const char         *mHostName;        ///< The host name (does not include domain name).
    const otIp6Address *mAddresses;       ///< Array of IPv6 host addresses.
    uint16_t            mAddressesLength; ///< Number of entries in @p mAddresses array.
    uint32_t            mTtl;             ///< The host TTL in seconds.
    uint32_t            mInfraIfIndex;    ///< The infrastructure network interface index.
} otPlatDnssdHost;

/**
 * Represents a DNS-SD key record.
 *
 * See `otPlatDnssdRegisterKey()`, `otPlatDnssdUnregisterKey()` for more details about fields in each case.
 *
 */
typedef struct otPlatDnssdKey
{
    const char    *mName;          ///< A host or a service instance name (does not include domain name).
    const char    *mServiceType;   ///< The service type if key is for a service (does not include domain name).
    const uint8_t *mKeyData;       ///< Byte array containing the key record data.
    uint16_t       mKeyDataLength; ///< Length of @p mKeyData in bytes.
    uint16_t       mClass;         ///< The resource record class.
    uint32_t       mTtl;           ///< The TTL in seconds.
    uint32_t       mInfraIfIndex;  ///< The infrastructure network interface index.
} otPlatDnssdKey;

/**
 * Callback to notify state changes of the DNS-SD platform.
 *
 * The OpenThread stack will call `otPlatDnssdGetState()` (from this callback or later) to get the new state. The
 * platform MUST therefore ensure that the returned state from `otPlatDnssdGetState()` is updated before calling this.
 *
 * @param[in] aInstance The OpenThread instance structure.
 *
 */
extern void otPlatDnssdStateHandleStateChange(otInstance *aInstance);

/**
 * Gets the current state of the DNS-SD module.
 *
 * The platform MUST notify the OpenThread stack whenever its state gets changed by invoking
 * `otPlatDnssdStateHandleStateChange()`.
 *
 * @param[in] aInstance     The OpenThread instance.
 *
 * @returns The current state of the DNS-SD module.
 *
 */
otPlatDnssdState otPlatDnssdGetState(otInstance *aInstance);

/**
 * Registers or updates a service on the infrastructure network's DNS-SD module.
 *
 * The @p aService and all its contained information (strings and buffers) are only valid during this call. The
 * platform MUST save a copy of the information if it wants to retain the information after returning from this
 * function.
 *
 * The fields in @p aService follow these rules:
 *
 * - The `mServiceInstance` and `mServiceType` fields specify the service instance label and service type name,
 *   respectively. They are never NULL.
 * - The `mHostName` field specifies the host name of the service if it is not NULL. Otherwise, if it is NULL, it
 *   indicates that this service is for the device itself and leaves the host name selection to DNS-SD platform.
 * - The `mSubTypeLabels` is an array of strings representing sub-types associated with the service. It can be NULL
 *   if there are no sub-types. Otherwise, the array length is specified by `mSubTypeLabelsLength`.
 * - The `mTxtData` and `mTxtDataLength` fields specify the encoded TXT data.
 * - The `mPort`, `mWeight`, and `mPriority` fields specify the service's parameters (as specified in DNS SRV record).
 * - The `mTtl` field specifies the TTL if non-zero. If zero, the platform can choose the TTL to use.
 * - The `mInfraIfIndex` field, if non-zero, specifies the infrastructure network interface index to use for this
 *   request. If zero, the platform implementation can decided the interface.
 *
 * When the `mHostName` field in @p aService is not NULL (indicating that this registration is on behalf of another
 * host), the OpenThread stack will ensure that `otPlatDnssdRegisterHost()` is also called for the same host before any
 * service registration requests for the same host.
 *
 * Once the registration request is finished, either successfully or failed, the platform reports the outcome by
 * invoking the @p aCallback and passing the same @p aRequestId in the callback. The @p aCallback function pointer can
 * be NULL, which indicates that the OpenThread stack does not need to be notified of the outcome of the request.
 * If the outcome is determined, the platform implementation may invoke the @p aCallback before returning from this
 * function. The OpenThread stack will ensure to handle such a situation.
 *
 * On success, the @p aCallback MUST be called (if non-NULL) with `OT_ERROR_NONE` as the `aError` input parameter. If
 * the registration causes a name conflict on DNS-SD domain (the service instance name is already claimed by another
 * host), the `OT_ERROR_DUPLICATED` error MUST be used. The platform implementation can use other `OT_ERROR` types for
 * other types of errors.
 *
 * The platform implementation MUST not assume that the @p aRequestId used in subsequent requests will be different.
 * OpenThread may reuse the same request ID again for a different request.
 *
 * The OpenThread stack will not register the same service (with no changes) that was registered successfully earlier.
 * Therefore, the platform implementation does not need to check for duplicate/same service and can assume that calls
 * to this function are either registering a new entry or changing some parameter in a previously registered item. As
 * a result, these changes always need to be synced on the infrastructure DNS-SD module.
 *
 * The OpenThread stack does not require the platform implementation to always invoke the @p aCallback function.
 * The OpenThread stack has its own mechanism to time out an aged request with no response. This relaxes the
 * requirement for platform implementations.
 *
 * @param[in] aInstance     The OpenThread instance.
 * @param[in] aService      Information about the service to register.
 * @param[in] aRequestId    The ID associated with this request.
 * @param[in] aCallback     The callback function pointer to report the outcome (may be NULL if no callback needed).
 *
 */
void otPlatDnssdRegisterService(otInstance                 *aInstance,
                                const otPlatDnssdService   *aService,
                                otPlatDnssdRequestId        aRequestId,
                                otPlatDnssdRegisterCallback aCallback);

/**
 * Unregisters a service on the infrastructure network's DNS-SD module.
 *
 * The @p aService and all its contained information (strings and buffers) are only valid during this call. The
 * platform MUST save a copy of the information if it wants to retain the information after returning from this
 * function.
 *
 * The fields in @p aService follow these rules:
 *
 * - The `mServiceInstance` and `mServiceType` fields specify the service instance label and service type name,
 *   respectively. They are never NULL.
 * - The `mHostName` field specifies the host name of the service if it is not NULL. Otherwise, if it is NULL, it
 *   indicates that this service is for the device itself and leaves the host name selection to DNS-SD platform.
 * - The `mInfraIfIndex` field, if non-zero, specifies the infrastructure network interface index to use for this
 *   request. If zero, the platform implementation can decided the interface.
 * - The rest of the fields in @p aService structure MUST be ignored in `otPlatDnssdUnregisterService()` call and may
 *   be set to zero by the OpenThread stack.
 *
 * Regarding the invocation of the @p aCallback and the reuse of the @p aRequestId, this function follows the same
 * rules as described in `otPlatDnssdRegisterService()`.
 *
 * The OpenThread stack may request the unregistration of a service that was not previously registered, and the
 * platform implementation MUST handle this case. In such a case, the platform can use either `OT_ERROR_NOT_FOUND` to
 * indicate that there was no such registration, or `OT_ERROR_NONE` when invoking the @p aCallback function. The
 * OpenThread stack will handle either case correctly.
 *
 * @param[in] aInstance     The OpenThread instance.
 * @param[in] aService      Information about the service to unregister.
 * @param[in] aRequestId    The ID associated with this request.
 * @param[in] aCallback     The callback function pointer to report the outcome (may be NULL if no callback needed).
 *
 */
void otPlatDnssdUnregisterService(otInstance                 *aInstance,
                                  const otPlatDnssdService   *aService,
                                  otPlatDnssdRequestId        aRequestId,
                                  otPlatDnssdRegisterCallback aCallback);

/**
 * Registers or updates a host on the infrastructure network's DNS-SD module.
 *
 * The @p aHost and all its contained information (strings and arrays) are only valid during this call. The
 * platform MUST save a copy of the information if it wants to retain the information after returning from this
 * function.
 *
 * The fields in @p aHost follow these rules:
 *
 * - The `mHostName` field specifies the host name to register. It is never NULL.
 * - The `mAddresses` field is an array of IPv6 addresses to register with the host. `mAddressesLength` field provides
 *   the number of entries in `mAddresses` array. The platform implementation MUST not filter or remove any of
 *   addresses in the list.
 *   The OpenThread stack will already ensure that the given addresses are externally reachable. For example, when
 *   registering host from an SRP registration, link-local or mesh-local addresses associated with the host which are
 *   intended for use within Thread mesh are not included in `mAddresses` array passed to this API. The `mAddresses`
 *   array can be empty with zero `mAddressesLength`. In such a case, the platform MUST stop advertising any addresses
 *   for this host name on the infrastructure DNS-SD.
 * - The `mTtl` field specifies the TTL if non-zero. If zero, the platform can choose the TTL to use.
 * - The `mInfraIfIndex` field, if non-zero, specifies the infrastructure network interface index to use for this
 *   request. If zero, the platform implementation can decided the interface.
 *
 * Regarding the invocation of the @p aCallback and the reuse of the @p aRequestId, this function follows the same
 * rules as described in `otPlatDnssdRegisterService()`.
 *
 * The OpenThread stack will not register the same host (with no changes) that was registered successfully earlier.
 * Therefore, the platform implementation does not need to check for duplicate/same host and can assume that calls
 * to this function are either registering a new entry or changing some parameter in a previously registered item. As
 * a result, these changes always need to be synced on the infrastructure DNS-SD module.
 *
 * @param[in] aInstance     The OpenThread instance.
 * @param[in] aHost         Information about the host to register.
 * @param[in] aRequestId    The ID associated with this request.
 * @param[in] aCallback     The callback function pointer to report the outcome (may be NULL if no callback needed).
 *
 */
void otPlatDnssdRegisterHost(otInstance                 *aInstance,
                             const otPlatDnssdHost      *aHost,
                             otPlatDnssdRequestId        aRequestId,
                             otPlatDnssdRegisterCallback aCallback);

/**
 * Unregisters a host on the infrastructure network's DNS-SD module.
 *
 * The @p aHost and all its contained information (strings and arrays) are only valid during this call. The
 * platform MUST save a copy of the information if it wants to retain the information after returning from this
 * function.
 *
 * The fields in @p aHost follow these rules:
 *
 * - The `mHostName` field specifies the host name to unregister. It is never NULL.
 * - The `mInfraIfIndex` field, if non-zero, specifies the infrastructure network interface index to use for this
 *   request. If zero, the platform implementation can decided the interface.
 * - The rest of the fields in @p aHost structure MUST be ignored in `otPlatDnssdUnregisterHost()` call and may
 *   be set to zero by the OpenThread stack.
 *
 * Regarding the invocation of the @p aCallback and the reuse of the @p aRequestId, this function follows the same
 * rules as described in `otPlatDnssdRegisterService()`.
 *
 * The OpenThread stack may request the unregistration of a host that was not previously registered, and the platform
 * implementation MUST handle this case. In such a case, the platform can use either `OT_ERROR_NOT_FOUND` to indicate
 * that there was no such registration, or `OT_ERROR_NONE` when invoking the @p aCallback function. OpenThread stack
 * will handle either case correctly.
 *
 * When unregistering a host, the OpenThread stack will also unregister any previously registered services
 * associated with the same host (by calling `otPlatDnssdUnregisterService()`). However, the platform implementation
 * MAY assume that unregistering a host also unregisters all its associated services.
 *
 * @param[in] aInstance     The OpenThread instance.
 * @param[in] aHost         Information about the host to unregister.
 * @param[in] aRequestId    The ID associated with this request.
 * @param[in] aCallback     The callback function pointer to report the outcome (may be NULL if no callback needed).
 *
 */
void otPlatDnssdUnregisterHost(otInstance                 *aInstance,
                               const otPlatDnssdHost      *aHost,
                               otPlatDnssdRequestId        aRequestId,
                               otPlatDnssdRegisterCallback aCallback);

/**
 * Registers or updates a key record on the infrastructure network's DNS-SD module.
 *
 * The @p aKey and all its contained information (strings and arrays) are only valid during this call. The
 * platform MUST save a copy of the information if it wants to retain the information after returning from this
 * function.
 *
 * The fields in @p aKey follow these rules:
 *
 * - If the key is associated with a host, `mName` field specifies the host name and `mServiceType` will be NULL.
 * - If the key is associated with a service, `mName` field specifies the service instance label and `mServiceType`
 *   field specifies the service type. In this case the DNS name for key record is `{mName}.{mServiceTye}`.
 * - The `mKeyData` field contains the key record's data with `mKeyDataLength` as its length in byes. It is never NULL.
 * - The `mClass` fields specifies the resource record class to use when registering key record.
 * - The `mTtl` field specifies the TTL if non-zero. If zero, the platform can choose the TTL to use.
 * - The `mInfraIfIndex` field, if non-zero, specifies the infrastructure network interface index to use for this
 *   request. If zero, the platform implementation can decided the interface.
 *
 * Regarding the invocation of the @p aCallback and the reuse of the @p aRequestId, this function follows the same
 * rules as described in `otPlatDnssdRegisterService()`.
 *
 * The OpenThread stack will not register the same key (with no changes) that was registered successfully earlier.
 * Therefore, the platform implementation does not need to check for duplicate/same name and can assume that calls
 * to this function are either registering a new key or changing the key data in a previously registered one. As
 * a result, these changes always need to be synced on the infrastructure DNS-SD module.
 *
 * @param[in] aInstance     The OpenThread instance.
 * @param[in] aHost         Information about the key record to register.
 * @param[in] aRequestId    The ID associated with this request.
 * @param[in] aCallback     The callback function pointer to report the outcome (may be NULL if no callback needed).
 *
 */
void otPlatDnssdRegisterKey(otInstance                 *aInstance,
                            const otPlatDnssdKey       *aKey,
                            otPlatDnssdRequestId        aRequestId,
                            otPlatDnssdRegisterCallback aCallback);

/**
 * Unregisters a key record on the infrastructure network's DNS-SD module.
 *
 * The @p aKey and all its contained information (strings and arrays) are only valid during this call. The
 * platform MUST save a copy of the information if it wants to retain the information after returning from this
 * function.
 *
 * The fields in @p aKey follow these rules:
 *
 * - If the key is associated with a host, `mName` field specifies the host name and `mServiceType` will be NULL.
 * - If the key is associated with a service, `mName` field specifies the service instance label and `mServiceType`
 *   field specifies the service type. In this case the DNS name for key record is `{mName}.{mServiceTye}`.
 * - The `mInfraIfIndex` field, if non-zero, specifies the infrastructure network interface index to use for this
 *   request. If zero, the platform implementation can decided the interface.
 * - The rest of the fields in @p aKey structure MUST be ignored in `otPlatDnssdUnregisterKey()` call and may
 *   be set to zero by the OpenThread stack.
 *
 * Regarding the invocation of the @p aCallback and the reuse of the @p aRequestId, this function follows the same
 * rules as described in `otPlatDnssdRegisterService()`.
 *
 * The OpenThread stack may request the unregistration of a key that was not previously registered, and the platform
 * implementation MUST handle this case. In such a case, the platform can use either `OT_ERROR_NOT_FOUND` to indicate
 * that there was no such registration, or `OT_ERROR_NONE` when invoking the @p aCallback function. the OpenThread
 * stack will handle either case correctly.
 *
 * @param[in] aInstance     The OpenThread instance.
 * @param[in] aKey          Information about the key to unregister.
 * @param[in] aRequestId    The ID associated with this request.
 * @param[in] aCallback     The callback function pointer to report the outcome (may be NULL if no callback needed).
 *
 */
void otPlatDnssdUnregisterKey(otInstance                 *aInstance,
                              const otPlatDnssdKey       *aKey,
                              otPlatDnssdRequestId        aRequestId,
                              otPlatDnssdRegisterCallback aCallback);

/**
 * @}
 *
 */

#ifdef __cplusplus
} // extern "C"
#endif

#endif // OPENTHREAD_PLATFORM_DNSSD_H_
