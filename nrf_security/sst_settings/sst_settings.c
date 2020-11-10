#include <zephyr.h>
#include <logging/log.h>
#include <settings/settings.h>

#include "sst_settings.h"

LOG_MODULE_REGISTER(sst_settings, CONFIG_SST_SETTINGS_LOG_LEVEL);

#define ROOT_KEY "SST"
#define MAX_PATH_LEN 32

struct read_ctx {
	/* Buffer for the setting. */
	void *buff;

	/* Buffer length. */
	size_t buff_len;

	/* Operation result. */
	int status;
};

static int read_cb(const char *name, size_t len, settings_read_cb read_cb,
		   void *cb_arg, void *param)
{
	int ret;
	const char *next;
	size_t name_len;
	struct read_ctx *ctx = (struct read_ctx *)param;

	__ASSERT(ctx->buff != NULL, "Improper value");

	name_len = settings_name_next(name, &next);
	if (name_len == 0 && len <= ctx->buff_len) {
		ret = read_cb(cb_arg, ctx->buff, len);
		if (ret <= 0) {
			LOG_ERR("Failed to read the setting, ret: %d", ret);
		}

		ctx->status = ret;

		return 0;
	}
	LOG_WRN("The setting is not present in the storage.");

	/* The settings is not found, return error and stop processing */
	return -ENOENT;
}

static int delete_cb(const char *key, size_t len, settings_read_cb read_cb,
		     void *cb_arg, void *param)
{
	int ret;
	char path[MAX_PATH_LEN];

	ARG_UNUSED(len);
	ARG_UNUSED(read_cb);
	ARG_UNUSED(cb_arg);
	ARG_UNUSED(param);

	ret = snprintk(path, sizeof(path), "%s/%s", ROOT_KEY, key);
	__ASSERT(ret < sizeof(path), "Setting path buffer too small.");

	LOG_DBG("Removing: %s", log_strdup(path));

	return settings_delete(path);
}

int sst_settings_init(void)
{
	return settings_subsys_init();
}

int sst_settings_get(uint32_t key, void *buff, size_t buff_len)
{
	int ret;
	char path[MAX_PATH_LEN];
	struct read_ctx read_ctx = {
		.buff = buff,
		.buff_len = buff_len,
		.status = -ENOENT,
	};

	LOG_DBG("Getting key: 0x%x", key);

	ret = snprintk(path, sizeof(path), "%s/%x", ROOT_KEY, key);
	__ASSERT(ret < sizeof(path), "Setting path buffer too small.");

	ret = settings_load_subtree_direct(path, read_cb, &read_ctx);
	if (ret != 0) {
		LOG_ERR("Failed to load setting key %d, ret %d", key, ret);
	}

	return read_ctx.status;
}

int sst_settings_set(uint32_t key, const void *buff, size_t buff_len)
{
	int ret;
	char path[MAX_PATH_LEN];

	LOG_DBG("Setting key: 0x%x", key);

	ret = snprintk(path, sizeof(path), "%s/%x", ROOT_KEY, key);
	__ASSERT(ret < sizeof(path), "Setting path buffer too small.");

	ret = settings_save_one(path, buff, buff_len);
	if (ret != 0) {
		LOG_ERR("Failed to store setting %d, ret %d", key, ret);
		return -1;
	}

	return 0;
}

int sst_settings_delete(uint32_t key)
{
	int ret;
	char path[MAX_PATH_LEN];

	ret = snprintk(path, sizeof(path), "%s/%x", ROOT_KEY, key);
	__ASSERT(ret < sizeof(path), "Setting path buffer too small.");

	LOG_DBG("Removing: %s", log_strdup(path));

	return settings_delete(path);
}

int sst_settings_delete_all(void)
{
	LOG_DBG("Removing all keys from: %s", ROOT_KEY);

	return settings_load_subtree_direct(ROOT_KEY, delete_cb, NULL);
}
