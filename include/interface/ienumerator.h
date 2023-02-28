#ifndef __DAST_ENUMERATOR_H__
#define __DAST_ENUMERATOR_H__

typedef struct _dast_enumerator_t dast_enumerator_t;

typedef struct _dast_enumerator_t
{
    int (*next)(void* self);
    void* (*current)(void* self);
    void (*reset)(void* self);
} _dast_enumerator_t;

#endif /* __DAST_ENUMERATOR_H__ */