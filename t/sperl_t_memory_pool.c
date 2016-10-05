#include <stdio.h>

#include "../sperl_memory_pool.h"
#include "../sperl_memory_pool_page.h"

#define OK(condition) \
  if (condition) {\
    printf("OK\n");\
  }\
  else {\
    printf("Not OK at line %d\n", __LINE__);\
  }

struct test {
  SPerl_int a;
  SPerl_double b;
  SPerl_long c;
};

int main(int argc, char *argv[])
{
  // new and free
  {
    SPerl_MEMORY_POOL* memory_pool = SPerl_MEMORY_POOL_new(0);
    
    SPerl_int* int_ptr1 = (SPerl_int*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(SPerl_int));
    SPerl_int* int_ptr2 = (SPerl_int*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(SPerl_int));
    
    *int_ptr1 = 1;
    *int_ptr2 = 2;
    
    OK(*int_ptr1 == ((SPerl_int*)memory_pool->page->data)[0]);
    OK(*int_ptr2 == ((SPerl_int*)memory_pool->page->data)[1]);
    
    SPerl_MEMORY_POOL_free(memory_pool);
  }

  // default values
  {
    SPerl_MEMORY_POOL* memory_pool = SPerl_MEMORY_POOL_new(sizeof(struct test));
    
    struct test* int_ptr1 = (struct test*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(struct test));
    struct test* int_ptr2 = (struct test*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(struct test));
    struct test* int_ptr3 = (struct test*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(struct test));
    struct test* int_ptr4 = (struct test*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(struct test));
    struct test* int_ptr5 = (struct test*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(struct test));
    struct test* int_ptr6 = (struct test*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(struct test));
    struct test* int_ptr7 = (struct test*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(struct test));
    struct test* int_ptr8 = (struct test*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(struct test));

    OK(int_ptr1->a == 0);
    OK(int_ptr2->a == 0);
    OK(int_ptr3->a == 0);
    OK(int_ptr4->a == 0);
    OK(int_ptr5->a == 0);
    OK(int_ptr6->a == 0);
    OK(int_ptr7->a == 0);
    OK(int_ptr8->a == 0);
    OK(int_ptr1->b == 0);
    OK(int_ptr2->b == 0);
    OK(int_ptr3->b == 0);
    OK(int_ptr4->b == 0);
    OK(int_ptr5->b == 0);
    OK(int_ptr6->b == 0);
    OK(int_ptr7->b == 0);
    OK(int_ptr8->b == 0);
    OK(int_ptr1->b == 0);
    OK(int_ptr2->b == 0);
    OK(int_ptr3->b == 0);
    OK(int_ptr4->b == 0);
    OK(int_ptr5->b == 0);
    OK(int_ptr6->b == 0);
    OK(int_ptr7->b == 0);
    OK(int_ptr8->b == 0);
  }
  
  // Crete next memroy node
  {
    SPerl_MEMORY_POOL* memory_pool = SPerl_MEMORY_POOL_new(sizeof(SPerl_int));
    
    SPerl_int* int_ptr1 = (SPerl_int*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(SPerl_int));
    SPerl_int* int_ptr2 = (SPerl_int*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(SPerl_int));
    SPerl_int* int_ptr3 = (SPerl_int*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(SPerl_int));
    SPerl_int* int_ptr4 = (SPerl_int*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(SPerl_int));
    SPerl_int* int_ptr5 = (SPerl_int*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(SPerl_int));
    SPerl_int* int_ptr6 = (SPerl_int*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(SPerl_int));
    SPerl_int* int_ptr7 = (SPerl_int*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(SPerl_int));
    SPerl_int* int_ptr8 = (SPerl_int*)SPerl_MEMORY_POOL_alloc(memory_pool, sizeof(SPerl_int));

    OK(*int_ptr1 == 0);
    OK(*int_ptr2 == 0);
    OK(*int_ptr3 == 0);
    OK(*int_ptr4 == 0);
    OK(*int_ptr5 == 0);
    OK(*int_ptr6 == 0);
    OK(*int_ptr7 == 0);
    OK(*int_ptr8 == 0);
    
    *int_ptr1 = 1;
    *int_ptr2 = 2;
    *int_ptr3 = 3;
    *int_ptr4 = 4;
    *int_ptr5 = 5;
    *int_ptr6 = 6;
    *int_ptr7 = 7;
    *int_ptr8 = 8;
    
    OK(*int_ptr1 == ((SPerl_int*)memory_pool->page->next->next->next->data)[0]);
    OK(*int_ptr2 == ((SPerl_int*)memory_pool->page->next->next->data)[0]);
    OK(*int_ptr3 == ((SPerl_int*)memory_pool->page->next->next->data)[1]);
    OK(*int_ptr4 == ((SPerl_int*)memory_pool->page->next->data)[0]);
    OK(*int_ptr5 == ((SPerl_int*)memory_pool->page->next->data)[1]);
    OK(*int_ptr6 == ((SPerl_int*)memory_pool->page->next->data)[2]);
    OK(*int_ptr7 == ((SPerl_int*)memory_pool->page->next->data)[3]);
    OK(*int_ptr8 == ((SPerl_int*)memory_pool->page->data)[0]);
    
    SPerl_MEMORY_POOL_free(memory_pool);
  }
}