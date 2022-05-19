
#ifndef ITER_H
#define ITER_h 1

struct iter
{
    int(*buffer_start);                                   // 8 bytes
    int(*buffer_end);                                     // 8 bytes
    int(*buffer_ptr_iter);                                // 8 bytes
    int buffer_size;                                      // 4 bytes
    int (*next)(struct iter(*));                          // 8 bytes
    void (*filter)(struct iter(*), int (*func_ptr)(int)); // 8 bytes
    void (*map)(struct iter(*), int (*func_ptr)(int));    // 8 bytes
    int buffer[];                                         // zero sized array (size determine at runtime)
};

extern struct iter *iter_init_seperate(int(*), int);
extern struct iter *iter_init_stack_depend(int(*), int);

// member functions
extern int next(struct iter(*));
extern void filter(struct iter(*), int (*func_ptr)(int));
extern void map(struct iter(*), int (*func_ptr)(int));

#endif