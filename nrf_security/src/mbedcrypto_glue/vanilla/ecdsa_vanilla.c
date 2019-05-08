#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(CONFIG_VANILLA_MBEDTLS_ECDSA_C) && defined(CONFIG_GLUE_MBEDTLS_ECDSA_C)

#include <assert.h>

#include "mbedtls/ecdsa.h"
#include "backend_ecdsa.h"


static int mbedtls_ecdsa_check(mbedtls_ecp_group *grp, mbedtls_ecp_group_id gid, int function)
{
    return 1;
}

const mbedtls_ecdsa_funcs mbedtls_ecdsa_vanilla_mbedtls_backend_funcs = {
    .check = mbedtls_ecdsa_check,
    .sign = mbedtls_ecdsa_sign,
    .verify = mbedtls_ecdsa_verify,
    .genkey = mbedtls_ecdsa_genkey,
};

#endif

