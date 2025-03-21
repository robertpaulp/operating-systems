// SPDX-License-Identifier: BSD-3-Clause

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "./threads.h"
#include "utils/utils.h"

#define NUM_ITER		10000000
#define NUM_THREADS		2

static void *printer_thread(void *arg)
{
	size_t id = (size_t)arg;
	int i;

	for (i = 0; i < NUM_ITER; ++i)
		if (!(i % 1000000))
			printf("Thread %zu: i = %d\n", id, i);

	return NULL;
}

int main(void)
{
	size_t i;
	int threads[NUM_THREADS];
	void *res;
	int rc;

	puts("Hello, this is the _main() func.");

	for (i = 0; i < NUM_THREADS; ++i) {
		threads[i] = threads_create(printer_thread, (void *)i);
		DIE(threads[i] < 0, "threads_create");
	}

	for (i = 0; i < NUM_THREADS; ++i)
		while (1) {
			rc = threads_join(threads[i], &res);
			DIE(rc < 0, "threads_join");

			if (rc) {
				printf("Joined thread %d with result %p\n",
					threads[i], res);
				break;
			}
		}

	return 0;
}
