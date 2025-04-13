#ifndef INCLUDE_PAGING_H
#define INCLUDE_PAGING_H

struct page {
    unsigned int present    : 1;
    unsigned int rw         : 1;
    unsigned int user       : 1;
    unsigned int accessed   : 1;
    unsigned int dirty      : 1;
    unsigned int unused     : 7;
    unsigned int frame      : 20;
};

void set_frame(unsigned int frame_addr);
void clear_frame(unsigned int frame_addr);
unsigned int test_frame(unsigned int frame_addr);
unsigned int first_frame();
void alloc_frame(struct page *page, int is_kernel, int is_writeable);
void free_frame(struct page *page);

#endif /* INCLUDE_PAGING_H */
