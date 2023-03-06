#ifndef __DAST_ITERATOR_H__
#define __DAST_ITERATOR_H__

typedef struct _dast_iterator_t dast_iterator_t;

typedef struct _dast_iterator_t
{
    int (*next)(void* self);
    void* (*current)(void* self);
    void (*reset)(void* self);
} _dast_iterator_t;

#endif /* __DAST_ITERATOR_H__ */