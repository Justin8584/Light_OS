#include "../include/stdlib.h"
#include "../include/paging.h"

extern unsigned int end;
unsigned int placement_address = (unsigned int)&end;

void *malloc(size_t size)
{
    unsigned int tmp = placement_address;
    placement_address += size;
    return (void *)tmp;
}

void free(void *ptr)
{
    (void)ptr;
}
