#ifndef __DAST_IITERATOR_H__
#define __DAST_IITERATOR_H__

typedef struct _dast_iterator_t
{
    void* (*next)(void* self);
    void (*reset)(void* self);
} dast_iterator_t;

#endif /* __DAST_IITERATOR_H__ */