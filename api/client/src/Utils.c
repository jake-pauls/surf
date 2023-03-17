#include "surf/Utils.h"
#include "surf/Define.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

char** StringSplit(char* str, const char delimeter)
{
    char** result = 0;
    size_t count = 0;
    char* lastDelimeter = 0;
    char delimeterArr[2] = { delimeter, 0 };

    // Count elements in the string
    char* tmp = str;
    while (*tmp)
    {
        if (delimeter == *tmp)
        {
            count++;
            lastDelimeter = tmp;
        }

        tmp++;
    }

    // Add space for trailing token
    count += lastDelimeter < (str + strlen(str) - 1);

    // Add space for null termination
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t index  = 0;
        char* nextToken = NULL;
        char* token = STRTOK(str, delimeterArr, &nextToken);

        while (token)
        {
            *(result + index++) = STRDUP(token);
            token = STRTOK(0, delimeterArr, &nextToken);
        }

        *(result + index) = 0;
    }

    return result;
}