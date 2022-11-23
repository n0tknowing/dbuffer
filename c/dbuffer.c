/* useful for:
 *  - append block of small/medium memory buffers, and merged them into
 *    one large memory (make sure your have enough memory to hold it)
 *  - recycle buffer with only 1 allocation and size less than
 *    or equal to initial 'buf_capacity'
 */

#include <err.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "dbuffer.h"

#define DBUF_MIN(x, y)	((x) < (y) ? (x) : (y))
#define DBUF_MAX(x, y)	((x) > (y) ? (x) : (y))

static size_t BUF_MAX_SZ_HARD = LONG_MAX;
typedef unsigned char u8;

void dbuffer_create1(struct dbuffer *dbuf, size_t max_sz)
{
	if (dbuf == NULL)
		return;

	max_sz = DBUF_MIN(max_sz, BUF_MAX_SZ_HARD);

	u8 *buf = calloc(max_sz, sizeof(u8));
	if (buf == NULL)
		return;

	dbuf->buf_data = buf;
	dbuf->buf_size = 0;
	dbuf->buf_capacity = max_sz;
}

void dbuffer_create(struct dbuffer *dbuf)
{
	dbuffer_create1(dbuf, 8192);
}

int dbuffer_grow(struct dbuffer *dbuf, size_t new_max_sz)
{
	if (dbuf == NULL)
		return -1;

	if (new_max_sz <= dbuf->buf_capacity)
		return 0;
	else if (dbuf->buf_capacity >= BUF_MAX_SZ_HARD)
		errx(1, "dbuffer_grow: out of memory (%zu bytes)", dbuf->buf_capacity);

	new_max_sz = DBUF_MIN(new_max_sz, BUF_MAX_SZ_HARD);
	u8 *new_buf = realloc(dbuf->buf_data, new_max_sz);
	if (new_buf == NULL)
		return -1;

	memset(new_buf + dbuf->buf_capacity, 0, new_max_sz - dbuf->buf_capacity);

	dbuf->buf_data = new_buf;
	dbuf->buf_capacity = new_max_sz;
	return 0;
}

void dbuffer_shrink_to_fit(struct dbuffer *dbuf)
{
	if (dbuf == NULL)
		return;

	if (dbuf->buf_size == 0 || dbuf->buf_size == dbuf->buf_capacity)
		return;

	size_t new_max_sz = dbuf->buf_size;
	u8 *new_buf = realloc(dbuf->buf_data, new_max_sz);
	if (new_buf == NULL)
		return;

	dbuf->buf_data = new_buf;
	dbuf->buf_capacity = new_max_sz;
}

int dbuffer_push(struct dbuffer *dbuf, const void *new_buf, size_t buf_sz)
{
	if (dbuf == NULL || new_buf == NULL)
		return -1;

	buf_sz = DBUF_MIN(buf_sz, BUF_MAX_SZ_HARD);
	size_t diff = dbuf->buf_capacity - dbuf->buf_size;
	if (buf_sz >= diff) {
		size_t new_cap = DBUF_MAX(buf_sz * 2, dbuf->buf_capacity * 2);
		if (dbuffer_grow(dbuf, new_cap) == -1)
			return -1;
	}

	memcpy((u8 *)dbuf->buf_data + dbuf->buf_size, new_buf, buf_sz);
	dbuf->buf_size += buf_sz;
	return 0;
}

const void *dbuffer_get_data(struct dbuffer *dbuf)
{
	if (dbuf == NULL)
		return NULL;

	return (const void *)dbuf->buf_data;
}

size_t dbuffer_get_size(struct dbuffer *dbuf)
{
	if (dbuf == NULL)
		return 0;

	return dbuf->buf_size;
}

size_t dbuffer_get_capacity(struct dbuffer *dbuf)
{
	if (dbuf == NULL)
		return 0;

	return dbuf->buf_capacity;
}

void dbuffer_clear(struct dbuffer *dbuf)
{
	if (dbuf == NULL)
		return;

	memset(dbuf->buf_data, 0, dbuf->buf_size);
	dbuf->buf_size = 0;
}

void dbuffer_destroy(struct dbuffer *dbuf)
{
	if (dbuf == NULL)
		return;

	free(dbuf->buf_data);

	dbuf->buf_data = NULL;
	dbuf->buf_size = 0;
	dbuf->buf_capacity = 0;
}
