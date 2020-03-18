#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

// Struct of double linked list - memory
typedef struct memory_list {

    void *address;
    size_t size;
    size_t is_busy;
    struct memory_list *prev;
    struct memory_list *next;

} memory_list;
//global memory - initialize in main function
memory_list *starting_memory;
//create memory block
memory_list *create_block(size_t size, size_t is_fill);
//adds memory when global memory runs out
memory_list *add_memory(size_t size);
// find node by address
memory_list *find_address_block(void *address);
//find first free_node->size < size
memory_list *find_first_free_block(size_t size);


void *mem_alloc(size_t size);
void *mem_realloc(void *address, size_t size);
void mem_free(void *address);
void union_mem();
void mem_dump();
void delete_block(memory_list *mem_block);