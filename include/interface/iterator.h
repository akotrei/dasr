#ifndef __DAST_ITERATOR_H__
#define __DAST_ITERATOR_H__

typedef struct _dast_iterator_t
{
    void* (*next)(void* self);
    void* (*prev)(void* self);
    void (*reset)(void* self);
} dast_iterator_t;

#endif /* __DAST_ITERATOR_H__ */