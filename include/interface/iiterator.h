#ifndef __DAST_IITERATOR_H__
#define __DAST_IITERATOR_H__

typedef struct _dast_iiterator_t dast_iiterator_t;

typedef struct _dast_iiterator_t
{
    int (*next)(void* self);
    void* (*current)(void* self);
    void (*reset)(void* self);
} _dast_iiterator_t;

#endif /* __DAST_IITERATOR_H__ */