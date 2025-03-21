// SPDX-License-Identifier: BSD-3-Clause

#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#include "utils/utils.h"

#define NUM_ITER 20000000

static int val;

void *increment_var(void *arg)
{
	(void)arg;

	for (size_t i = 0; i < NUM_ITER; i++)
		atomic_fetch_add(&val, 1);

	return NULL;
}

void *decrement_var(void *arg)
{
	(void)arg;

	for (size_t i = 0; i < NUM_ITER; i++)
		/* TODO 1: Use `atomic_fetch_sub` to implement `var -= 1` atomically. */
		atomic_fetch_sub(&val, 1);

	return NULL;
}

int main(void)
{
	int rc;
	pthread_t tids[2];

	rc = pthread_create(tids, NULL, increment_var, NULL);
	DIE(rc < 0, "pthread_create");
	rc = pthread_create(tids + 1, NULL, decrement_var, NULL);
	DIE(rc < 0, "pthread_create");

	rc = pthread_join(tids[0], NULL);
	DIE(rc < 0, "pthread_join");
	rc = pthread_join(tids[1], NULL);
	DIE(rc < 0, "pthread_join");

	printf("var = %d\n", val);

	return 0;
}
