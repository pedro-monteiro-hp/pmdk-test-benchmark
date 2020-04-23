// PMDKBenchmarks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <libpmemobj.h>
#include <time.h>

#define L"test" "test" /* will use this in create and open */

POBJ_LAYOUT_BEGIN(test);
POBJ_LAYOUT_ROOT(test, struct my_root);
POBJ_LAYOUT_TOID(test, int)
POBJ_LAYOUT_TOID(test, void)
POBJ_LAYOUT_END(test);

struct my_root {
	PMEMoid array[35];
};

void shuffle(int *array, size_t n)
{
	if (n > 1)
	{
		size_t i;
		for (i = 0; i < n - 1; i++)
		{
			size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
			int t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
	}
}

void read_random_1mb(PMEMobjpool *pop)
{
	int index_array_root[35], index_array_buffer[4096];
	int i, j;

	for (i = 0; i < 35; i++)
	{
		index_array_root[i] = i;
	}
	for (i = 0; i < 4096; i++)
	{
		index_array_buffer[i] = i;
	}

	shuffle(index_array_root, 35);
	shuffle(index_array_buffer, 4096);

	clock_t t;

	t = clock();
	PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	struct my_root* my_root_p = (struct my_root*)pmemobj_direct(root);
	if (OID_IS_NULL(root)) {
		printf("root oid null\n");
	}

	size_t increment = 1024 * 1024;
	char* buffer = (char*)malloc(increment);
	for (i = 0; i < 35; i++) {
		char* p = (char*)pmemobj_direct(my_root_p->array[index_array_root[i]]);
		for (j = 0; j < 4096; j++) {
			memcpy(buffer, p + (index_array_buffer[j] * increment), increment);
		}
	}
	t = clock() - t;
	double time_taken_read = ((double)t) / CLOCKS_PER_SEC;

	printf("read random 140GB in 1mb took %f seconds\n", time_taken_read);

}

void read_random_512mb(PMEMobjpool *pop)
{
	int index_array_root[35], index_array_buffer[8];
	int i, j;

	for (i = 0; i < 35; i++)
	{
		index_array_root[i] = i;
	}
	for (i = 0; i < 8; i++)
	{
		index_array_buffer[i] = i;
	}

	shuffle(index_array_root, 35);
	shuffle(index_array_buffer, 8);

	clock_t t;

	t = clock();
	PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	struct my_root* my_root_p = (struct my_root*)pmemobj_direct(root);
	if (OID_IS_NULL(root)) {
		printf("root oid null\n");
	}

	size_t increment = 1024 * 1024 * 512;
	char* buffer = (char*)malloc(increment);
	for (i = 0; i < 35; i++) {
		char* p = (char*)pmemobj_direct(my_root_p->array[index_array_root[i]]);
		for (j = 0; j < 8; j++) {
			memcpy(buffer, p + (index_array_buffer[j] * increment), increment);
		}
	}
	t = clock() - t;
	double time_taken_read = ((double)t) / CLOCKS_PER_SEC;

	printf("read random 140GB in 512mb took %f seconds\n", time_taken_read);

}

void write_random_1mb(PMEMobjpool *pop)
{
	int index_array_root[35], index_array_buffer[4096];
	int i, j;

	for (i = 0; i < 35; i++)
	{
		index_array_root[i] = i;
	}
	for (i = 0; i < 4096; i++)
	{
		index_array_buffer[i] = i;
	}

	shuffle(index_array_root, 35);
	shuffle(index_array_buffer, 4096);

	clock_t t;

	t = clock();
	PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	struct my_root* my_root_p = (struct my_root*)pmemobj_direct(root);
	if (OID_IS_NULL(root)) {
		printf("root oid null\n");
	}

	size_t increment = 1025 * 1024;
	for (i = 0; i < 35; i++) {
		char* p = (char*)pmemobj_direct(my_root_p->array[index_array_root[i]]);
		for (j = 0; j < 4096; j++) {
			pmemobj_memset(pop, p + (index_array_buffer[j] * increment), 0, increment, 0);
		}
	}
	t = clock() - t;
	double time_taken_write = ((double)t) / CLOCKS_PER_SEC;

	printf("write random 140GB in 1mb took %f seconds\n", time_taken_write);
}

void write_random_512mb(PMEMobjpool *pop)
{
	int index_array_root[35], index_array_buffer[8];
	int i, j;

	for (i = 0; i < 35; i++)
	{
		index_array_root[i] = i;
	}
	for (i = 0; i < 8; i++)
	{
		index_array_buffer[i] = i;
	}

	shuffle(index_array_root, 35);
	shuffle(index_array_buffer, 8);

	clock_t t;
	t = clock();
	
	PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	struct my_root* my_root_p = (struct my_root*)pmemobj_direct(root);
	if (OID_IS_NULL(root)) {
		printf("root oid null\n");
	}

	size_t increment = 1025 * 1024 * 512;
	for (i = 0; i < 35; i++) {
		char* p = (char*)pmemobj_direct(my_root_p->array[index_array_root[i]]);
		for (j = 0; j < 8; j++) {
			pmemobj_memset(pop, p + (index_array_buffer[j] * increment), 0, increment, 0);
		}
	}
	t = clock() - t;
	double time_taken_write = ((double)t) / CLOCKS_PER_SEC;
	
	printf("write random 140GB in 512mb took %f seconds\n", time_taken_write);

}

void read_sequential_1mb(PMEMobjpool *pop)
{
	clock_t t;

	t = clock();
	PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	struct my_root* my_root_p = (struct my_root*)pmemobj_direct(root);
	if (OID_IS_NULL(root)) {
		printf("root oid null\n");
	}


	int i, j;
	size_t increment = 1024 * 1024;
	char* buffer = (char*)malloc(increment);
	for (i = 0; i < 35; i++) {
		char* p = (char*)pmemobj_direct(my_root_p->array[i]);
		for (j = 0; j < 4096; j++) {
			memcpy(buffer, p + (j*increment), increment);
		}
	}
	t = clock() - t;
	double time_taken_read = ((double)t) / CLOCKS_PER_SEC;

	printf("read 140GB in 1mb took %f seconds\n", time_taken_read);

}

void read_sequential_512mb(PMEMobjpool *pop)
{
	clock_t t;

	t = clock();
	PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	struct my_root* my_root_p = (struct my_root*)pmemobj_direct(root);
	if (OID_IS_NULL(root)) {
		printf("root oid null\n");
	}


	int i, j;
	size_t increment = 1024 * 1024 * 512;
	char* buffer = (char*)malloc(increment);
	for (i = 0; i < 35; i++) {
		char* p = (char*)pmemobj_direct(my_root_p->array[i]);
		for (j = 0; j < 8; j++) {
			memcpy(buffer, p + (j*increment), increment);
		}
	}
	t = clock() - t;
	double time_taken_read = ((double)t) / CLOCKS_PER_SEC;

	printf("read 140GB in 512mb took %f seconds\n", time_taken_read);


}

void write_sequential_1mb(PMEMobjpool *pop)
{
	clock_t t;

	t = clock();
	PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	struct my_root* my_root_p = (struct my_root*)pmemobj_direct(root);
	if (OID_IS_NULL(root)) {
		printf("root oid null\n");
	}


	int i, j;
	size_t increment = 1024 * 1024;
	for (i = 0; i < 35; i++) {
		char* p = (char*)pmemobj_direct(my_root_p->array[i]);
		for (j = 0; j < 4096; j++) {
			pmemobj_memset(pop, p + (j*increment), 0, increment, 0);
		}
	}
	t = clock() - t;
	double time_taken_write = ((double)t) / CLOCKS_PER_SEC;

	printf("write 140GB in 1mb took %f seconds\n", time_taken_write);


}

void write_sequential_512mb(PMEMobjpool *pop)
{
	clock_t	t;
	
	t = clock();
	PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	struct my_root* my_root_p = (struct my_root*)pmemobj_direct(root);
	if (OID_IS_NULL(root)) {
		printf("root oid null\n");
	}
	
	int i, j;
	size_t increment = 1024 * 1024 * 512;
	for (i = 0; i < 35; i++) {
		char* p = (char*)pmemobj_direct(my_root_p->array[i]);
		for (j = 0; j < 8; j++) {
			pmemobj_memset(pop, p + (j*increment), 0, increment, 0);
		}
	}
	t = clock() - t;
	double time_taken_write = ((double)t) / CLOCKS_PER_SEC;

	printf("write 140GB in 512mb took %f seconds\n", time_taken_write);

}

void pool()
{
	clock_t t;
	t = clock();

	size_t size = (size_t)1024 * 1024 * 1024 * 200;
	PMEMobjpool *pop = pmemobj_create(L"D:/pmem.pool", L"test", size, 0666);
	if (pop == NULL) {
		perror("pmemobj_create");
		return;
	}
	t = clock() - t;
	double time_taken_create = ((double)t) / CLOCKS_PER_SEC;

	t = clock();
	PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	struct my_root* my_root_p = (struct my_root*)pmemobj_direct(root);
	if (OID_IS_NULL(root)) {
		printf("root oid null\n");
	}


	int i;
	for (i = 0; i < 35; i++) {
		POBJ_ALLOC(pop, &(my_root_p->array[i]), void, (size_t)4 * 1024 * 1024 * 1024, NULL, NULL);
		if (OID_IS_NULL(my_root_p->array[i])) {
			printf("array oid null\n");
		}
	}
	t = clock() - t;
	double time_taken_alloc = ((double)t) / CLOCKS_PER_SEC;

	printf("create pool took %f seconds\n", time_taken_create);
	printf("allocate 140GB took %f seconds\n", time_taken_alloc);

	pmemobj_close(pop);

	write_sequential_1mb(pop);
	write_sequential_512mb(pop);
	write_random_1mb(pop);
	write_random_512mb(pop);
	read_sequential_1mb(pop);
	read_sequential_512mb(pop);
	read_random_1mb(pop);
	read_random_512mb(pop);
}

void poolset()
{
	clock_t t;
	t = clock();

	size_t size = (size_t)1024 * 1024 * 1024 * 200;
	size_t growsize = (size_t)1024 * 1024 * 1024;
	uint64_t* granularity = (uint64_t*)malloc(sizeof(uint64_t));

	PMEMobjpool *pop = pmemobj_create(L"D:/pool.set", L"test", 0, 0666);
	if (pop == NULL) {
		perror("pmemobj_create");
		return;
	}

	*granularity = (uint64_t)growsize;
	if (pmemobj_ctl_set(pop, L"heap.size.granularity", granularity) == -1)
	{
		printf("failed setting\n");
	}
	else
	{
		printf("set pool granularity size as %ld bytes\n", *granularity);
	}

	t = clock() - t;
	double time_taken_create = ((double)t) / CLOCKS_PER_SEC;

	t = clock();
	PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	struct my_root* my_root_p = (struct my_root*)pmemobj_direct(root);
	if (OID_IS_NULL(root)) {
		printf("root oid null\n");
	}


	int i;
	for (i = 0; i < 35; i++) {
		POBJ_ALLOC(pop, &(my_root_p->array[i]), void, (size_t)4 * 1024 * 1024 * 1024, NULL, NULL);
		if (OID_IS_NULL(my_root_p->array[i])) {
			printf("array oid null\n");
		}
	}
	t = clock() - t;
	double time_taken_alloc = ((double)t) / CLOCKS_PER_SEC;

	printf("create pool took %f seconds\n", time_taken_create);
	printf("allocate 140GB took %f seconds\n", time_taken_alloc);

	pmemobj_close(pop);

	write_sequential_1mb(pop);
	write_sequential_512mb(pop);
	write_random_1mb(pop);
	write_random_512mb(pop);
	read_sequential_1mb(pop);
	read_sequential_512mb(pop);
	read_random_1mb(pop);
	read_random_512mb(pop);
}



int main(int argc, char *argv[])
{
	pool();

	if (remove(L"D:/pmem.pool") != 0)
		perror("Error deleting file");
	else
		puts("File successfully deleted");

	printf("-------------------------\n");
	poolset();

	return 0;
}


