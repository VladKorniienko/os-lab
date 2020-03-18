
#include "allocator.h"

int main()
{
	void* data;
	void* data1;
	void* data2;
	void* data3;


	init_memory(MEMORY_SIZE);
	data = mem_alloc(200);
	memmove(data, "Hello World", 11);
	data1 = mem_alloc(100);
	memmove(data1, "My Lab", 6);
	data2 = mem_alloc(100);
	memmove(data2, "Hey", 3);
	data3 = mem_alloc(500);
	memmove(data3, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
	aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
	aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
	aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
	aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
	aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
	aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
	aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
	aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
	aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
	aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 500);
	mem_dump();
	mem_realloc(data2,100);
	mem_free(data);
	mem_dump();
	return 0;
}