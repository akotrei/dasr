#ifndef __DAST_IITERATOR_H__
#define __DAST_IITERATOR_H__

typedef struct _dast_iterator_t
{
    int (*next)(void* self);
    void* (*current)(void* self);
    void (*reset)(void* self);
} dast_iterator_t;

#endif /* __DAST_IITERATOR_H__ */