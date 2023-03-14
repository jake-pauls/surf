#include "surf/Utils.h"

#include <stdlib.h>
#include <string.h>

char* strndup(const char* buffer, size_t n)
{
    char *p;
    size_t n1;

    for (n1 = 0; n1 < n && buffer[n1] != '\0'; n1++)
    {
        continue;
    }

    p = malloc(n + 1);

    if (p != NULL) 
    {
        memcpy(p, buffer, n1);
        p[n1] = '\0';
    }

    return p;
}
