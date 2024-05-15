#include "debug.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    printf("Hello world!\n");
    printf("argc: %d\n", argc);
    int i;
    for (i = 0; i < argc; i++)
    {
        printf("argv #%d: %s\n", i, argv[i]);
    }
    DEBUG_PRINT("Debugging is enabled\n");
    assert(1 == 1);
    return 0;
}
