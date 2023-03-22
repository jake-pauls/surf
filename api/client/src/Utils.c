#include "surf/Utils.h"
#include "surf/Define.h"

#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/stat.h>

time_t GetFileLastModifiedTime(const char* filepath)
{
    struct stat fileStat;
    int err = stat(filepath, &fileStat);
    if (err != 0)
        SURF_API_CLIENT_LOG("Failed to retrieve file stats for %s", filepath);

    return fileStat.st_mtime;
}

int IsFileModified(const char* filepath, time_t lastModifiedTime) 
{
    struct stat fileStat;
    int err = stat(filepath, &fileStat);
    if (err != 0) 
        SURF_API_CLIENT_LOG("Failed to retrieve file stats for %s", filepath);

    return fileStat.st_mtime > lastModifiedTime;
}

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

void StringToLower(char** str) 
{
    if (IsStringEmpty(*(str)))
        return;

    for (char* p = *(str); *p; p++)
    {
        *p = tolower(*p);
    }
}

int IsStringEmpty(char* str)
{
    return str && !str[0];
}

int IsStringInt(char* str)
{
    int len;
    int buffer;

    // Scan for an int and check the original string for excess chars
    int result = sscanf(str, "%d %n", &buffer, &len);
    return result && !str[len];
}

int IsStringFloat(char* str)
{
    int len;
    float buffer;

    // Scan for a float and check the original string for excess chars
    int result = sscanf(str, "%f %n", &buffer, &len);
    return result && !str[len];
}

#ifdef WIN32
int vasprintf(char** pStr, const char* fmt, ...)
{
    va_list ap;
    
    // Retrieve variadic args
    va_start(ap, fmt);

    // Get required length of the string
    int len = _vscprintf(fmt, ap);
    if (len == -1) 
        return -1;

    // Allocate memory
    size_t size = (size_t) len + 1;
    char* str = malloc(size);
    if (!str)
        return -1;

    // Write bytes into the string
    int bytes = vsprintf_s(str, len + 1, fmt, ap);
    if (bytes == -1) 
    {
        free(str);
        return -1;
    }

    *pStr = str;

    return bytes;
}
#endif
