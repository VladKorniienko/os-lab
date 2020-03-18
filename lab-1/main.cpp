
#include "allocator.h"
#include <string.h>

int main()
{

 int choice;
 void *address;
 size_t size;
 //char data[2048];

     starting_memory = create_block(2048, 0);
  printf("Your initial statistics:");
  /*void * var = mem_alloc(5);
  void * var1 = mem_alloc(1);
  mem_dump();
  mem_realloc(var, 8);
  mem_dump();
  mem_realloc(var1, 10);
  mem_dump();
  mem_free(var);
  union_mem();
  mem_dump();*/
 
 
  
 
    while (1)
 {
  
   mem_dump();
   printf("\n1) Add memmory Block\n2) Realloc memmory Block\n3) Delete memomry Block\n4) Exit the Allocator\n------------------\n");
   printf("Input your number please: ");

   scanf_s("%i", &choice);
  
  if(choice == 1){
   printf("Input size: ");
   scanf_s("%lu", &size);
   mem_alloc(size);
  }
  else if(choice == 2){
   printf("Input address: ");
   scanf_s("%p", &address);
   printf("Input new size: ");
   scanf_s("%lu", &size);
   mem_realloc(address, size);
  }
  else if(choice == 3){
   printf("Input address: ");
   scanf_s("%p", &address);
   mem_free(address);
   union_mem();
  }
  else if (choice == 4)
  {
   exit(0);
  }
  
 }
 return (0);
}