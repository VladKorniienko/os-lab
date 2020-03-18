#include "allocator.h"

memory_list *create_block(size_t size, size_t is_fill)
{

    memory_list *memory_current_block;
    memory_current_block = (memory_list *)malloc(sizeof(memory_list));
    
    if (memory_current_block == NULL || size == 0){ return NULL;}
    

    memory_current_block->size = size;
    memory_current_block->address = is_fill ? malloc(size * sizeof(void*)) : NULL;
    memory_current_block->is_busy = !is_fill;
    memory_current_block->prev = NULL;
    memory_current_block->next = NULL;

    return memory_current_block;
}

memory_list *add_memory(size_t size)
{

    memory_list *begin;
    memory_list *mem_block;

    begin = starting_memory;
    mem_block = create_block(size, 1);

    while (begin->next != NULL)
    {
        begin = begin->next;
    }

    begin->next = mem_block;
    mem_block->prev = begin;

    return mem_block;
}

memory_list *find_address_block(void *address)
{

    memory_list *current_block;

    current_block = starting_memory;

    while (current_block != NULL)
    {

        if (current_block->address == address)
        {
            return current_block;
        }

        current_block = current_block->next;
    }

    return NULL;
}

memory_list *find_first_free_block(size_t size)
{

    memory_list *current_block;

    current_block = starting_memory;

    while (current_block != NULL)
    {

        if (current_block->is_busy == 1 && size <= current_block->size)
        {
            return current_block;
        }

        current_block = current_block->next;
    }

    return NULL;
}



//delete block of memory, used in mem_free function
void delete_block(memory_list* mem_block) {

    if (mem_block->next != NULL) {
        mem_block->next->prev = mem_block->prev;
    }
    if (mem_block->prev != NULL) {
        mem_block->prev->next = mem_block->next;
    }
    else {
        starting_memory = mem_block->next;
    }

    // free all block and data
    free(mem_block->address);
    free(mem_block);
}

//reallocate memory block if it valid. If not, it will allocate memory with the specified size
void* mem_realloc(void* address, size_t size) {

    if (address == NULL) {
        return mem_alloc(size);
    }

    memory_list* old_block;
    memory_list* new_block;

    old_block = find_address_block(address);

    if (old_block != NULL && size != 0) {

        if (find_first_free_block(size) != NULL) {
            find_first_free_block(size)->size += old_block->size < size ? 0 - (size - old_block->size) : old_block->size - size;
        }
        new_block = create_block(size, 1);
        new_block->address = memcpy(new_block->address, old_block->address, old_block->size > size ? size : old_block->size);

        new_block->next = old_block->next;
        new_block->prev = old_block->prev;

        if (old_block->prev != NULL) {
            old_block->prev->next = new_block;
        }
        else
        {
            starting_memory = new_block;
        }

        if (old_block->next != NULL) {
            old_block->next->prev = new_block;
        }

        free(old_block->address);
        free(old_block);

        return new_block->address;

    }
    else {
        return NULL;
    }
}

//Clears the memory and removes the block.
void mem_free(void* address) {

    memory_list* mem_block;

    mem_block = find_address_block(address);

    if (mem_block != NULL) {
        mem_block->is_busy = 1;
        free(mem_block->address);
        mem_block->address = NULL;

        if (mem_block->prev != NULL && mem_block->prev->is_busy) {
            mem_block->prev->size += mem_block->size;
        }
        else if (mem_block->next != NULL && mem_block->next->is_busy) {
            mem_block->next->size += mem_block->size;
        }
        else {
            return;
        }
        delete_block(mem_block);
    }
    else {
        return;
    }

}
// if size != 0 allocate memory. Create block 
void* mem_alloc(size_t size) {

    if (size == 0) {
        return NULL;
    }

    memory_list* make_newblock;
    memory_list* block_extract;

    make_newblock = find_first_free_block(size);

    if (make_newblock != NULL) {
        make_newblock->address = malloc(size * sizeof(void*));
        make_newblock->is_busy = 0;
        int z = (make_newblock->size - size);
        if (abs(z) > 0) {
            block_extract = create_block(make_newblock->size - size, 0);

            if (make_newblock->next != NULL) {
                make_newblock->next->prev = block_extract;
            }
            block_extract->next = make_newblock->next;
            block_extract->prev = make_newblock;
            make_newblock->next = block_extract;
        }
        make_newblock->size = size;
        return (make_newblock->address);
    }
    else {
        return (add_memory(size)->address);
    }

}

//displays the memory stats
void mem_dump()
{
    memory_list* begin;

    begin = starting_memory;
    printf("STATISTICS:\n");
    while (begin)
    {
        printf("Address: %15p| Size: %15lu\n"/* | Data: %15s\n"*/, begin->address, begin->size/*, (char *)begin->address*/);
        begin = begin->next;
    }
}

void union_mem() {
    memory_list* begin = starting_memory;
    while (begin != NULL) {
        if (begin->is_busy == 1) {
            if (begin->prev != NULL && begin->prev->is_busy) {
                begin->prev->size += begin->size;
                delete_block(begin);
            }
            else if (begin->next != NULL && begin->next->is_busy) {
                begin->next->size += begin->size;
                delete_block(begin);
            }

        }

        begin = begin->next;
    }

}