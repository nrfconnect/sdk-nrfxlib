#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(CONFIG_VANILLA_MBEDTLS_ECDH_C) && defined(CONFIG_GLUE_MBEDTLS_ECDH_C)

#include <assert.h>

#include "mbedtls/ecdh.h"
#include "backend_ecdh.h"


static int mbedtls_ecdh_check(mbedtls_ecp_group *grp, int function)
{
    return 1;
}

const mbedtls_ecdh_funcs mbedtls_ecdh_vanilla_mbedtls_backend_funcs = {
    .check = mbedtls_ecdh_check,
    .gen_public = mbedtls_ecdh_gen_public,
    .compute_shared = mbedtls_ecdh_compute_shared,
};

#endif

