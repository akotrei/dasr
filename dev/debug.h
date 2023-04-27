#ifndef __DAST_DEBUG_H__
#define __DAST_DEBUG_H__

#include <stdio.h>
#include <assert.h>

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)                                              \
    fprintf(stderr,                                                            \
            "DEBUG: %s:%d:%s(): " fmt,                                         \
            __FILE__,                                                          \
            __LINE__,                                                          \
            __func__,                                                          \
            ##args)
#else
#define DEBUG_PRINT(fmt, args...) /* Don't do anything in release builds */
#endif

#endif /* __DAST_DEBUG_H__ */
