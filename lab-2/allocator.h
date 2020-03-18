#ifndef ALLOCATOR
# define ALLOCATOR
#include <iostream>
#include <cstring>

#define MEMORY_SIZE 1340
#define PAGE_SIZE 256
#define PAGE_INFO 12

#define FREE 0
#define DIVIDE 1
#define BLOCK 2
#define MULTBLOCK 3


typedef struct s_page{

    void* address;
    char state;
    size_t size;

} mem_page;


void init_memory(size_t size);

void* mem_alloc(size_t size);
void* mem_realloc(void* address, size_t size);
void mem_free(void* address);

void mem_dump();

extern mem_page* global_memory;
extern size_t page_count;
#endif