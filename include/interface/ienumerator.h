#ifndef __DAST_IENUMERATOR_H__
#define __DAST_IENUMERATOR_H__

typedef struct _dast_ienumerator_t dast_ienumerator_t;

typedef struct _dast_ienumerator_t
{
    int (*next)(void* self);
    void* (*current)(void* self);
    void (*reset)(void* self);
} _dast_ienumerator_t;

#endif /* __DAST_IENUMERATOR_H__ */