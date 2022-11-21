#ifndef DBUFFER_H
#define DBUFFER_H

#include <stddef.h> // size_t

struct dbuffer {
	void *buf_data;
	size_t buf_size;
	size_t buf_capacity;
};

void dbuffer_create1(struct dbuffer *dbuf, size_t max_sz);
void dbuffer_create(struct dbuffer *dbuf);
int dbuffer_grow(struct dbuffer *dbuf, size_t new_max_sz);
void dbuffer_shrink(struct dbuffer *dbuf);
int dbuffer_push(struct dbuffer *dbuf, const void *new_buf, size_t buf_sz);
const void *dbuffer_get_data(struct dbuffer *dbuf);
size_t dbuffer_get_size(struct dbuffer *dbuf);
size_t dbuffer_get_capacity(struct dbuffer *dbuf);
void dbuffer_clear(struct dbuffer *dbuf);
void dbuffer_destroy(struct dbuffer *dbuf);

#endif
