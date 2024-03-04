/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem/include/nrf_errno.h
 *
 * @brief Defines integer values for errno.
 *        Used by system calls to indicates the latest error.
 */
#ifndef NRF_ERRNO_H__
#define NRF_ERRNO_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief nRF error numbers
 * A list of error codes defined by IEEE Std 1003.1-2017 found at:
 * https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 * @defgroup nrf_errno nRF error numbers
 * @{
 */

#define NRF_EPERM 1             /**< Operation not permitted */
#define NRF_ENOENT 2            /**< No such file or directory */
#define NRF_ESRCH 3             /**< No such process */
#define NRF_EINTR 4             /**< Interrupted function */
#define NRF_EIO 5               /**< I/O error */
#define NRF_ENXIO 6             /**< No such device or address */
#define NRF_E2BIG 7             /**< Argument list too long */
#define NRF_ENOEXEC 8           /**< Executable file format error */
#define NRF_EBADF 9             /**< Bad file descriptor */
#define NRF_ECHILD 10           /**< No child processes */
#define NRF_EAGAIN 11           /**< Resource unavailable, try again */
#define NRF_ENOMEM 12           /**< Not enough space */
#define NRF_EACCES 13           /**< Permission denied */
#define NRF_EFAULT 14           /**< Bad address */
#define NRF_EBUSY 16            /**< Device or resource busy */
#define NRF_EEXIST 17           /**< File exists */
#define NRF_EXDEV 18            /**< Cross-device link */
#define NRF_ENODEV 19           /**< No such device */
#define NRF_ENOTDIR 20          /**< Not a directory or a symbolic link to a directory */
#define NRF_EISDIR 21           /**< Is a directory */
#define NRF_EINVAL 22           /**< Invalid argument */
#define NRF_ENFILE 23           /**< Too many files open in system */
#define NRF_EMFILE 24           /**< File descriptor value too large */
#define NRF_ENOTTY 25           /**< Inappropriate I/O control operation */
#define NRF_ETXTBSY 26          /**< Text file busy */
#define NRF_EFBIG 27            /**< File too large */
#define NRF_ENOSPC 28           /**< No space left on device */
#define NRF_ESPIPE 29           /**< Invalid seek */
#define NRF_EROFS 30            /**< Read-only file system */
#define NRF_EMLINK 31           /**< Too many links */
#define NRF_EPIPE 32            /**< Broken pipe */
#define NRF_EDOM 33             /**< Mathematics argument out of domain of function */
#define NRF_ERANGE 34           /**< Result too large */
#define NRF_ENOMSG 35           /**< No message of the desired type */
#define NRF_EDEADLK 45          /**< Resource deadlock would occur */
#define NRF_ENOLCK 46           /**< No locks available */
#define NRF_ENOSTR 60           /**< Not a STREAM */
#define NRF_ENODATA 61          /**< No message is available on the STREAM head read queue */
#define NRF_ETIME 62            /**< Stream ioctl() timeout */
#define NRF_ENOSR 63            /**< No STREAM resources */
#define NRF_EPROTO 71           /**< Protocol error */
#define NRF_EBADMSG 77          /**< Bad message */
#define NRF_ENOSYS 88           /**< Functionality not supported */
#define NRF_ENOTEMPTY 90        /**< Directory not empty */
#define NRF_ENAMETOOLONG 91     /**< File name too long */
#define NRF_ELOOP 92            /**< Too many levels of symbolic links */
#define NRF_EOPNOTSUPP 95       /**< Operation not supported on socket */
#define NRF_ECONNRESET 104      /**< Connection reset */
#define NRF_ENOBUFS 105         /**< No buffer space available */
#define NRF_EAFNOSUPPORT 106    /**< Address family not supported */
#define NRF_EPROTOTYPE 107      /**< Protocol wrong type for socket */
#define NRF_ENOTSOCK 108        /**< Not a socket */
#define NRF_ENOPROTOOPT 109     /**< Protocol not available */
#define NRF_ECONNREFUSED 111    /**< Connection refused */
#define NRF_EADDRINUSE 112      /**< Address in use */
#define NRF_ECONNABORTED 113    /**< Connection aborted */
#define NRF_ENETUNREACH 114     /**< Network unreachable */
#define NRF_ENETDOWN 115        /**< Network is down */
#define NRF_ETIMEDOUT 116       /**< Connection timed out */
#define NRF_EHOSTUNREACH 118    /**< Host is unreachable */
#define NRF_EINPROGRESS 119     /**< Operation in progress */
#define NRF_EALREADY 120        /**< Connection already in progress */
#define NRF_EDESTADDRREQ 121    /**< Destination address required */
#define NRF_EMSGSIZE 122        /**< Message too large */
#define NRF_EPROTONOSUPPORT 123 /**< Protocol not supported */
#define NRF_EADDRNOTAVAIL 125   /**< Address not available */
#define NRF_ENETRESET 126       /**< Connection aborted by network */
#define NRF_EISCONN 127         /**< Socket is connected */
#define NRF_ENOTCONN 128        /**< The socket is not connected */
#define NRF_ENOTSUP 134         /**< Not supported */
#define NRF_EILSEQ 138          /**< Illegal byte sequence */
#define NRF_EOVERFLOW 139       /**< Value too large to be stored in data type */
#define NRF_ECANCELED 140       /**< Operation canceled */

#define NRF_EWOULDBLOCK NRF_EAGAIN  /**< Operation would block */

/* Not in IEEE Std 1003.1-2017 */

#define NRF_ENOTBLK 15          /**< Block device required */
#define NRF_EPFNOSUPPORT 96     /**< Protocol family not supported */
#define NRF_ESHUTDOWN 110       /**< Can't send after socket shutdown */
#define NRF_EHOSTDOWN 117       /**< Host is down */
#define NRF_ESOCKTNOSUPPORT 124 /**< Socket type not supported */
#define NRF_ETOOMANYREFS 129    /**< Too many references: can't splice */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* NRF_ERRNO_H__ */
