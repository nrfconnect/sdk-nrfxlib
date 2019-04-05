#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(CONFIG_VANILLA_MBEDTLS_CCM_C) && defined(CONFIG_GLUE_MBEDTLS_CCM_C)

#include <assert.h>

#include "mbedtls/ccm.h"
#include "mbedtls/ccm_alt.h"
#include "backend_ccm.h"


static_assert(MBEDTLS_DEFAULT_CCM_CONTEXT_WORDS == (sizeof(mbedtls_ccm_context) - 4 + 3) / 4, "Invalid MBEDTLS_DEFAULT_CCM_CONTEXT_WORDS value");


static int mbedtls_ccm_check(mbedtls_cipher_id_t cipher, unsigned int keybits)
{
    return 1;
}

const mbedtls_ccm_funcs mbedtls_ccm_default_backend_funcs = {
    .backend_context_size = (4 * MBEDTLS_DEFAULT_CCM_CONTEXT_WORDS),
    .check = mbedtls_ccm_check,
    .init = mbedtls_ccm_init,
    .setkey = mbedtls_ccm_setkey,
    .free = mbedtls_ccm_free,
    .encrypt_and_tag = mbedtls_ccm_encrypt_and_tag,
    .star_encrypt_and_tag = mbedtls_ccm_star_encrypt_and_tag,
    .auth_decrypt = mbedtls_ccm_auth_decrypt,
    .star_auth_decrypt = mbedtls_ccm_star_auth_decrypt,
};

#endif

