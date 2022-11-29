#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbuffer.h"

int main(void)
{
	struct dbuffer buffer;
	dbuffer_create(&buffer);

	size_t sz[6] = { 20000, 60000, 64, 1000, 5000, 8192 };
	for (int i = 0; i < 6; i++) {
		unsigned char *buf = malloc(sz[i]);
		if (buf == NULL) {
			fprintf(stderr, "malloc() %zu bytes failed\n", sz[i]);
			continue;
		}
		memset(buf, i, sz[i]);
		int rc = dbuffer_push(&buffer, buf, sz[i]);
		if (rc == -1) {
			printf("dbuffer_push() returned -1\n");
			free(buf);
			return 1;
		}
		printf("dbuffer_push() %zu bytes success\n", sz[i]);
		free(buf);
	}

	printf("dbuffer_size() %zu bytes\n", dbuffer_size(&buffer));
	printf("dbuffer_capacity() %zu bytes\n", dbuffer_capacity(&buffer));

	dbuffer_destroy(&buffer);
	return 0;
}
