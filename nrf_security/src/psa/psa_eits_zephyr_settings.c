/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#include <zephyr.h>
#include <init.h>
#include <logging/log.h>
#include <settings/settings.h>
#include <sys/printk.h>
#include <storage/flash_map.h>

#include <nrf_cc3xx_platform.h>
#include <nrf_cc3xx_platform_ctr_drbg.h>
#include <nrf_cc3xx_platform_derived_key.h>

#include "psa_eits_backend.h"

LOG_MODULE_REGISTER(psa_eits_zephyr_settings,
		    CONFIG_PSA_EITS_BACKEND_ZEPHYR_LOG_LEVEL);


const uint8_t deriv_context[]="static_deriv_context";

#define ROOT_KEY "EITS"
#define MAX_PATH_LEN 64
#define DERIV_KEY_SIZE 128
#define DERIV_KEY_SLOT  0
#define ENC_TAG_SIZE 16
#define ENC_IV_SIZE 16
#define ENC_BUFF_SIZE 1024
#define DERIV_LABEL_SIZE (sizeof(psa_storage_uid_t) + sizeof(deriv_context))
#define ENC_ADD_DATA_SIZE (sizeof(psa_storage_uid_t)+ \
                           ENC_IV_SIZE+ \
                           sizeof(uint32_t)+ \
                           sizeof(psa_storage_create_flags_t))


static uint8_t encrypted_buffer[CONFIG_PSA_EITS_READ_BUFF_SIZE];
static uint8_t decrypted_buffer[CONFIG_PSA_EITS_READ_BUFF_SIZE];
static uint8_t deriv_label[DERIV_LABEL_SIZE];
static uint8_t tag[ENC_TAG_SIZE];
static uint8_t iv[ENC_IV_SIZE];
static uint8_t add_data[ENC_ADD_DATA_SIZE];

K_MUTEX_DEFINE(eits_mutex);

typedef enum {
    PATH_TYPE_ROOT,
    PATH_TYPE_DATA,
    PATH_TYPE_FLAGS,
    PATH_TYPE_IV,
    PATH_TYPE_TAG
} PATH_TYPES;


struct read_info_ctx {
    /* A container for metadata associated with a specific uid */
    struct psa_storage_info_t *info;

    /* Operation result. */
    psa_status_t status;
};

struct read_data_ctx {
    /* Buffer for the data associated with a specific uid */
    void *buff;

    /* Buffer length. */
    size_t buff_len;

    /* Operation result. */
    psa_status_t status;
};

static int read_info_cb( const char *name, size_t len, settings_read_cb read_cb,
                         void *cb_arg, void *param )
{
    int ret;
    const char *next;
    size_t next_len;
    struct read_info_ctx *ctx = (struct read_info_ctx *) param;

    __ASSERT( ctx != NULL, "Improper value" );
    __ASSERT( ctx->info != NULL, "Improper value" );

    next_len = settings_name_next( name, &next );

    if ( !strncmp( name, "data", next_len ) )
    {
        ctx->info->size = len;
        ctx->status = PSA_SUCCESS;
        LOG_DBG( "read: <%s_.size> = %d", name, len );

        return 0;
    }

    if ( !strncmp( name, "flags", next_len ) )
    {
        ret = read_cb( cb_arg, &ctx->info->flags, sizeof( ctx->info->flags ) );
        if ( ret <= 0 )
        {
            LOG_ERR( "Failed to read the setting, ret: %d", ret );
            ctx->status = PSA_ERROR_STORAGE_FAILURE;
        }
        ctx->status = PSA_SUCCESS;
        LOG_DBG( "read: <%s> = %d", name, ctx->info->flags );

        return 0;
    }

    if ( !strncmp( name, "iv", next_len ) || !strncmp( name, "tag", next_len ) ){
        ctx->status = PSA_SUCCESS;
        return 0;
    }

    return -ENOENT;
}

static int read_data_direct_cb( const char *name, size_t len, settings_read_cb read_cb,
                         void *cb_arg, void *param )
{
    int ret;
    const char *next;
    size_t next_len;
    struct read_data_ctx *ctx = (struct read_data_ctx *) param;

    __ASSERT( ctx != NULL, "Improper value" );
    __ASSERT( ctx->buff != NULL, "Improper value" );

    next_len = settings_name_next( name, &next );
    if ( next_len == 0 )
    {
        if ( len > ctx->buff_len )
        {
            ctx->status = PSA_ERROR_BUFFER_TOO_SMALL;
            return -ENOENT;
        }

        ret = read_cb( cb_arg, ctx->buff, len );
        if ( ret <= 0 )
        {
            LOG_ERR( "Failed to read the setting, ret: %d", ret );
            ctx->status = PSA_ERROR_STORAGE_FAILURE;
        }
        ctx->status = PSA_SUCCESS;
        LOG_DBG( "read: <%s/data>", name );

        return 0;
    }
    LOG_WRN( "The setting: %s is not present in the storage.", name );

    /* The settings is not found, return error and stop processing */
    return -ENOENT;
}

static psa_status_t is_write_once_set( psa_storage_uid_t uid,
                                       bool *is_write_once )
{
    psa_status_t status;
    struct psa_storage_info_t info = {
        .size = 0,
        .flags = 0,
    };

    __ASSERT( is_write_once != NULL, "Improper value" );

    status = psa_its_get_info( uid, &info );
    if ( status != PSA_SUCCESS )
    {
        return status;
    }

    *is_write_once = info.flags & PSA_STORAGE_FLAG_WRITE_ONCE;

    return PSA_SUCCESS;
}

static void create_settings_path( psa_storage_uid_t uid, char *buff,
                              size_t buff_size, PATH_TYPES path_type)
{
    __ASSERT( buff != NULL, "NULL buffer not allowed" );

    int ret;
    switch(path_type){
      case PATH_TYPE_ROOT:
        ret = snprintk( buff, buff_size, "%s/%" PRIx64, ROOT_KEY, uid );
      break;

      case PATH_TYPE_DATA:
        ret = snprintk( buff, buff_size, "%s/%" PRIx64 "/data", ROOT_KEY, uid );
      break;

      case PATH_TYPE_FLAGS:
        ret = snprintk( buff, buff_size, "%s/%" PRIx64 "/flags", ROOT_KEY, uid );
      break;

      case PATH_TYPE_IV:
        ret = snprintk( buff, buff_size, "%s/%" PRIx64 "/iv", ROOT_KEY, uid );
      break;

      case PATH_TYPE_TAG:
        ret = snprintk( buff, buff_size, "%s/%" PRIx64 "/tag", ROOT_KEY, uid );
      break;
    }

    __ASSERT( ret < buff_size, "Buffer size is too small." );
}

static void fill_deriv_label(psa_storage_uid_t uid,
                            uint8_t *deriv_label,
                            size_t deriv_label_size)
{
    size_t ret;

    __ASSERT( deriv_label != NULL, "Improper value" );
    __ASSERT( deriv_label_size >=  DERIV_LABEL_SIZE, "Improper value" );

    ret = snprintk(deriv_label, deriv_label_size, "%" PRIx64 "%s", uid, deriv_context);

    __ASSERT( ret < expected_deriv_label_len, "Could not fill deriv_label." );
}

static void fill_add_data(psa_storage_uid_t uid,
                         psa_storage_create_flags_t create_flags,
                         uint32_t data_size,
                         uint8_t *iv,
                         size_t iv_len,
                         uint8_t *add_data,
                         size_t add_data_len)
{
    size_t buff_offset;

    __ASSERT( add_data != NULL, "Improper value" );
    __ASSERT( iv != NULL, "Improper value" );
    __ASSERT( add_data_len >=  ENC_ADD_DATA_SIZE, "Improper value" );

    buff_offset = 0;

    memcpy(add_data, &uid, sizeof(uid));
    buff_offset+=sizeof(uid);

    memcpy(add_data + buff_offset, iv, iv_len);
    buff_offset+=iv_len;

    memcpy(add_data + buff_offset, &data_size, sizeof(data_size));
    buff_offset+=sizeof(data_size);

    memcpy(add_data + buff_offset, &create_flags, sizeof(create_flags));
    buff_offset+=sizeof(create_flags);
}

static void zeroize_global_buffers(void)
{
    memset(encrypted_buffer, 0x0, CONFIG_PSA_EITS_READ_BUFF_SIZE);
    memset(decrypted_buffer, 0x0, CONFIG_PSA_EITS_READ_BUFF_SIZE);
    memset(deriv_label, 0x0, DERIV_LABEL_SIZE);
    memset(tag, 0x0, ENC_TAG_SIZE);
    memset(iv, 0x0, ENC_IV_SIZE);
    memset(add_data, 0x0, ENC_ADD_DATA_SIZE);
}

static psa_status_t crypt_data(psa_storage_uid_t uid,
                               psa_storage_create_flags_t create_flags,
                               const uint8_t *input,
                               uint32_t input_len,
                               uint8_t *output,
                               bool is_encrypt)
{
    nrf_cc3xx_platform_derived_key_ctx_t ctx;
    int ret;

    ret = nrf_cc3xx_platform_derived_key_init(&ctx);
    if (ret != NRF_CC3XX_PLATFORM_SUCCESS){
        return PSA_ERROR_STORAGE_FAILURE;
    }

    fill_deriv_label(uid, deriv_label, DERIV_LABEL_SIZE);

    ret = nrf_cc3xx_platform_derived_key_set_info(&ctx,
                                                  DERIV_KEY_SLOT,
                                                  DERIV_KEY_SIZE,
                                                  deriv_label,
                                                  DERIV_LABEL_SIZE);
    if (ret != NRF_CC3XX_PLATFORM_SUCCESS){
        return PSA_ERROR_STORAGE_FAILURE;
    }

    ret = nrf_cc3xx_platform_derived_key_set_cipher(&ctx,
                                                    ALG_CHACHAPOLY_256_BIT);
    if (ret != NRF_CC3XX_PLATFORM_SUCCESS){
        return PSA_ERROR_STORAGE_FAILURE;
    }

    fill_add_data(uid,
                  create_flags,
                  input_len,
                  iv,
                  ENC_IV_SIZE,
                  add_data,
                  ENC_ADD_DATA_SIZE);

    ret = nrf_cc3xx_platform_derived_key_set_auth_info(&ctx,
                                                       iv,
                                                       ENC_IV_SIZE,
                                                       add_data,
                                                       ENC_ADD_DATA_SIZE,
                                                       tag,
                                                       ENC_TAG_SIZE);
    if (ret != NRF_CC3XX_PLATFORM_SUCCESS){
        return PSA_ERROR_STORAGE_FAILURE;
    }

    if(is_encrypt){
        ret = nrf_cc3xx_platform_derived_key_encrypt(&ctx,
                                                     output,
                                                     input_len,
                                                     input);

    }else{
        ret = nrf_cc3xx_platform_derived_key_decrypt(&ctx,
                                                     output,
                                                     input_len,
                                                     input);
    }

    if (ret != NRF_CC3XX_PLATFORM_SUCCESS){
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static psa_status_t generate_iv(uint8_t *iv, size_t iv_size)
{
    int ret;
    size_t out __attribute__((unused));

    ret = nrf_cc3xx_platform_ctr_drbg_init(NULL, NULL, 0);
    if (ret != NRF_CC3XX_PLATFORM_SUCCESS){
        return PSA_ERROR_STORAGE_FAILURE;
    }

    ret = nrf_cc3xx_platform_ctr_drbg_get(NULL, iv, iv_size, &out);
    if (ret!= NRF_CC3XX_PLATFORM_SUCCESS){
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static psa_status_t read_data( psa_storage_uid_t uid,
                               PATH_TYPES path_type,
                               struct read_data_ctx *read_data_ctx )
{
    int ret;
    char path[MAX_PATH_LEN];

    create_settings_path(uid, path, sizeof(path), path_type);
    ret = settings_load_subtree_direct( path, read_data_direct_cb, read_data_ctx);
    if ( ret != 0 || read_data_ctx->status != PSA_SUCCESS)
    {
        LOG_ERR( "Failed to load data for uid: %" PRIx64 ", ret %d", uid, ret );
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;

}

static psa_status_t read_encrypted_data( psa_storage_uid_t uid,
                                         struct read_data_ctx *read_data_ctx )
{
    psa_status_t ret;

    struct read_data_ctx read_iv_ctx = {
        .buff = iv,
        .buff_len = sizeof(iv),
        .status = PSA_ERROR_DOES_NOT_EXIST,
    };

    struct read_data_ctx read_tag_ctx = {
        .buff = tag,
        .buff_len = sizeof(tag),
        .status = PSA_ERROR_DOES_NOT_EXIST,
    };

    struct read_data_ctx read_encrypted_data_ctx = {
        .buff = encrypted_buffer,
        .buff_len = sizeof(encrypted_buffer),
        .status = PSA_ERROR_DOES_NOT_EXIST,
    };

    psa_storage_create_flags_t create_flags;
    struct read_data_ctx read_flags_ctx = {
        .buff = &create_flags,
        .buff_len = sizeof(create_flags),
        .status = PSA_ERROR_DOES_NOT_EXIST,
    };

    if ( read_data_ctx->buff_len >= read_encrypted_data_ctx.buff_len){
        LOG_ERR( "CONFIG_PSA_EITS_READ_BUFF_SIZE too small");
        read_data_ctx->status = PSA_ERROR_STORAGE_FAILURE;
        return PSA_ERROR_STORAGE_FAILURE;
    }else{
        read_encrypted_data_ctx.buff_len = read_data_ctx->buff_len;
    }

    ret = read_data(uid, PATH_TYPE_IV, &read_iv_ctx);
    if( ret != PSA_SUCCESS ) {
        return ret;
    }

    ret = read_data(uid, PATH_TYPE_TAG, &read_tag_ctx);
    if( ret != PSA_SUCCESS ) {
        return ret;
    }

    ret = read_data(uid, PATH_TYPE_FLAGS, &read_flags_ctx);
    if( ret != PSA_SUCCESS ) {
        return ret;
    }

    ret = read_data(uid, PATH_TYPE_DATA, &read_encrypted_data_ctx);
    if( ret != PSA_SUCCESS ) {
        return ret;
    }

    ret = crypt_data(uid,
                     create_flags,
                     read_encrypted_data_ctx.buff,
                     read_encrypted_data_ctx.buff_len,
                     read_data_ctx->buff,
                     false);
    if ( ret != PSA_SUCCESS )
    {
        LOG_ERR( "Failed to decrypt data for uid: %" PRIx64 ", ret %d", uid, ret );
        read_data_ctx->status = PSA_ERROR_STORAGE_FAILURE;
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static psa_status_t read_data_with_offset( psa_storage_uid_t uid,
                                           uint32_t data_offset,
                                           uint32_t data_length, void *p_data,
                                           size_t *p_data_length,
                                           struct read_data_ctx *read_data_ctx )
{

    struct psa_storage_info_t info = {
        .size = 0,
        .flags = 0,
    };

    psa_status_t status = psa_its_get_info( uid, &info );
    if ( status != PSA_SUCCESS )
    {
        return status;
    }

    if ( data_offset >= info.size )
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if ( info.size - data_offset > data_length )
    {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    __ASSERT( CONFIG_PSA_EITS_READ_BUFF_SIZE >= info.size,
              "CONFIG_PSA_EITS_READ_BUFF_SIZE too small" );

    read_data_ctx->buff = decrypted_buffer;
    read_data_ctx->buff_len = info.size;

    status = read_encrypted_data( uid, read_data_ctx );
    if ( status == PSA_SUCCESS )
    {
        *p_data_length = info.size - data_offset;
        memcpy( p_data, decrypted_buffer + data_offset, *p_data_length );
    }

    return status;
}


psa_status_t psa_its_set_backend( psa_storage_uid_t uid, uint32_t data_length,
                                  const void *p_data,
                                  psa_storage_create_flags_t create_flags )
{
    char path[MAX_PATH_LEN];
    psa_status_t status;
    bool is_write_once;
    int ret;

    LOG_DBG( "Setting uid: %" PRIx64, uid );

    /* Check that the arguments are valid */
    if ( p_data == NULL || data_length == 0 )
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check that the create_flags does not contain any unsupported flags */
    if ( create_flags & ~( PSA_STORAGE_FLAG_WRITE_ONCE ) )
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Check that the create_flags does not contain any unsupported flags */
    if ( data_length > ENC_BUFF_SIZE )
    {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    status = is_write_once_set( uid, &is_write_once );
    if ( status != PSA_SUCCESS && status != PSA_ERROR_DOES_NOT_EXIST )
    {
        return status;
    }

    if ( status == PSA_SUCCESS && is_write_once )
    {
        return PSA_ERROR_NOT_PERMITTED;
    }

    /* Store flags associated with the stored data */
    create_settings_path(uid, path, sizeof(path), PATH_TYPE_FLAGS);
    if ( settings_save_one( path, (const void *) &create_flags,
                            sizeof( create_flags ) ) )
    {
        LOG_ERR( "Failed to store flags for uid: %" PRIx64, uid );
        return PSA_ERROR_STORAGE_FAILURE;
    }

    k_mutex_lock(&eits_mutex, K_FOREVER);

    /* Fill the IV here */
    ret = generate_iv(iv, ENC_IV_SIZE);
    if (ret != NRF_CC3XX_PLATFORM_SUCCESS){
        return PSA_ERROR_STORAGE_FAILURE;
    }

    /* Encrypt data */
    status = crypt_data(uid, create_flags, p_data, data_length, encrypted_buffer, true);
    if(status != PSA_SUCCESS){
        zeroize_global_buffers();
        k_mutex_unlock(&eits_mutex);
        return PSA_ERROR_STORAGE_FAILURE;
    }

    /* Store the encrypted data */
    create_settings_path(uid, path, sizeof(path), PATH_TYPE_DATA);
    if ( settings_save_one( path, encrypted_buffer, data_length ) )
    {
        LOG_ERR( "Failed to store data for uid: %" PRIx64, uid );
        zeroize_global_buffers();
        k_mutex_unlock(&eits_mutex);
        return PSA_ERROR_STORAGE_FAILURE;
    }

    /* Store the IV */
    create_settings_path(uid, path, sizeof(path), PATH_TYPE_IV);
    if ( settings_save_one( path, iv, ENC_IV_SIZE ) )
    {
        LOG_ERR( "Failed to store IV for uid: %" PRIx64, uid );
        zeroize_global_buffers();
        k_mutex_unlock(&eits_mutex);
        return PSA_ERROR_STORAGE_FAILURE;
    }

    /* Store the TAG */
    create_settings_path(uid, path, sizeof(path), PATH_TYPE_TAG);
    if ( settings_save_one( path, tag, ENC_TAG_SIZE ) )
    {
        LOG_ERR( "Failed to store TAG for uid: %" PRIx64, uid );
        zeroize_global_buffers();
        k_mutex_unlock(&eits_mutex);
        return PSA_ERROR_STORAGE_FAILURE;
    }

    zeroize_global_buffers();
    k_mutex_unlock(&eits_mutex);

    return PSA_SUCCESS;
}

psa_status_t psa_its_get_backend( psa_storage_uid_t uid, uint32_t data_offset,
                                  uint32_t data_length, void *p_data,
                                  size_t *p_data_length )
{
    psa_status_t status;

    struct read_data_ctx read_data_ctx = {
        .buff = p_data,
        .buff_len = data_length,
        .status = PSA_ERROR_DOES_NOT_EXIST,
    };

    LOG_DBG( "Getting data for uid: 0x%" PRIx64, uid );

    if ( p_data == NULL || p_data_length == NULL || data_length == 0 )
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    k_mutex_lock(&eits_mutex, K_FOREVER);

    if ( data_offset != 0 )
    {
        status = read_data_with_offset( uid, data_offset, data_length, p_data,
                                        p_data_length, &read_data_ctx );
    }
    else
    {
        status = read_encrypted_data( uid, &read_data_ctx );
        if ( status == PSA_SUCCESS )
        {
            *p_data_length = data_length;
        }

    }

    zeroize_global_buffers();
    k_mutex_unlock(&eits_mutex);

    return status;
}

psa_status_t psa_its_get_info_backend( psa_storage_uid_t uid,
                                       struct psa_storage_info_t *p_info )
{
    int ret;
    char path[MAX_PATH_LEN];
    struct read_info_ctx read_info_ctx = {
        .info = p_info,
        .status = PSA_ERROR_DOES_NOT_EXIST,
    };

    LOG_DBG( "Getting info for uid: 0x%" PRIx64, uid );

    if ( p_info == NULL )
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    create_settings_path(uid, path, sizeof(path), PATH_TYPE_ROOT);
    ret = settings_load_subtree_direct( path, read_info_cb, &read_info_ctx );
    if ( ret != 0 )
    {
        LOG_ERR( "Failed to load info for uid: %" PRIx64 ", ret %d", uid, ret );
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return read_info_ctx.status;
}

psa_status_t psa_its_remove_backend( psa_storage_uid_t uid )
{
    psa_status_t status;
    bool is_write_once;
    char path[MAX_PATH_LEN];

    LOG_DBG( "Removing data for uid: %" PRIx64, uid );

    status = is_write_once_set( uid, &is_write_once );
    if ( status != PSA_SUCCESS )
    {
        return status;
    }

    if ( is_write_once )
    {
        return PSA_ERROR_NOT_PERMITTED;
    }

    create_settings_path(uid, path, sizeof(path), PATH_TYPE_FLAGS);
    if ( settings_delete( path ) )
    {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    create_settings_path(uid, path, sizeof(path), PATH_TYPE_DATA);
    if ( settings_delete( path ) )
    {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    create_settings_path(uid, path, sizeof(path), PATH_TYPE_IV);
    if ( settings_delete( path ) )
    {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    create_settings_path(uid, path, sizeof(path), PATH_TYPE_TAG);
    if ( settings_delete( path ) )
    {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static int zephyr_settings_init( const struct device *device )
{
    int ret;

    ARG_UNUSED( device );

    ret = settings_subsys_init();
    if ( ret != 0 )
    {
        LOG_ERR( "settings_subsys_init failed (ret %d)", ret );
    }

    return ret;
}

SYS_INIT( zephyr_settings_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY );
