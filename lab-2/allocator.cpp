#include "allocator.h"

mem_page* global_memory;
size_t page_count;

void mem_dump()
{
	printf("------------Memory Dump------------\n");
	printf("Global Memory Address: %p\n", global_memory);
	for (size_t i = 0; i < page_count; i++)
	{
		printf("-----------------------------\n");
		printf("Page: %i State: %3i Address: %p\n", i, global_memory[i].state, global_memory[i].address);
		printf("Data: %s\n", (char*)global_memory[i].address);
		if (global_memory[i].state == BLOCK || global_memory[i].state == DIVIDE)
			printf("Data: %s\n", (char*)((size_t)global_memory[i].address + PAGE_SIZE / 2));
		printf("-----------------------------\n");
	}
}

void init_memory(size_t size)
{
	global_memory = (mem_page*)malloc(size);
	global_memory = (mem_page*)calloc(MEMORY_SIZE, 1);
	page_count = size / (PAGE_SIZE + PAGE_INFO);

	for (size_t i = 0; i < page_count; i++)
	{
		global_memory[i].address = (void*)((unsigned long int)global_memory + page_count * PAGE_INFO + i * PAGE_SIZE);
		global_memory[i].size = 0;
		global_memory[i].state = FREE;
	}
	
}

void* mem_alloc(size_t size)
{
	void* address;
	size_t counter = 0;
	size_t firstpage = 0;
	size_t pages_alloc = (double)size / PAGE_SIZE + 0.5;
	size_t i;
	if (size <= PAGE_SIZE)
	{
		for (i = 0; i < page_count; i++)
		{
			if (global_memory[i].state == FREE || (global_memory[i].state == DIVIDE && size <= PAGE_SIZE / 2))
			{
				address = (void*)((size_t)(global_memory[i].address) + (global_memory[i].state == DIVIDE ? PAGE_SIZE / 2 : 0));
				if (size > PAGE_SIZE / 2)
				{
					global_memory[i].state = BLOCK;
					global_memory[i].size = 1;
				}
				else
				{
					global_memory[i].state = global_memory[i].state == DIVIDE ? BLOCK : DIVIDE;
					global_memory[i].size = 0;
				}
				return address;
			}
		}
	}
	else
	{
		for (i = 0; i < page_count; i++)
		{
			if (global_memory[i].state == FREE)
			{
				counter += 1;
				firstpage = i - counter + 1;
				if (counter == pages_alloc)
				{
					for (size_t j = firstpage; j < firstpage + counter; j++)
					{
						global_memory[j].state = MULTBLOCK;
					}
					global_memory[firstpage].size = pages_alloc * PAGE_SIZE;
					address = global_memory[firstpage].address;
					return address;
				}
			}
			else
			{
				counter = 0;
				firstpage = 0;
			}
		}
	}
	return NULL;
}

void* mem_realloc(void* address, size_t size)
{
	void* new_address;
	size_t size_of_block = (size_t)address - (size_t)global_memory - PAGE_INFO * page_count;
	size_t started_block_page = size_of_block / PAGE_SIZE;
	if (global_memory[started_block_page].state == MULTBLOCK)
		size_of_block = global_memory[started_block_page].size;
	else if (global_memory[started_block_page].size != 0)
		size_of_block = global_memory[started_block_page].size * PAGE_SIZE;
	else
		size_of_block = PAGE_SIZE / 2;

	new_address = mem_alloc(size);
	if (new_address)
	{
		memmove(new_address, address, size_of_block > size ? size : size_of_block);
		mem_free(address);
		return new_address;
	}
	return NULL;
}

void mem_free(void* address)
{
	size_t size_of_block = (size_t)address - (size_t)global_memory - PAGE_INFO * page_count;
	size_t started_block_page = size_of_block / PAGE_SIZE;
	size_t page_state = global_memory[started_block_page].state;
	if (page_state == DIVIDE)
		global_memory[started_block_page].state = FREE;
	else if (page_state == BLOCK)
	{
		if (global_memory[started_block_page].size == 0)
			global_memory[started_block_page].state = DIVIDE;
		else
			global_memory[started_block_page].state = FREE;
	}
		
	else if (page_state == MULTBLOCK)
		for (size_t i = started_block_page; i <global_memory[started_block_page].size / PAGE_SIZE; i++)
			global_memory[i].state = FREE;
	std::memset(address, 0, global_memory[started_block_page].size == 0 ? PAGE_SIZE / 2: global_memory[started_block_page].size);
	global_memory[started_block_page].size = 0;
	address = NULL;


}