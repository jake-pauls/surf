#ifndef Utils_h
#define Utils_h

#include <stddef.h>

/// @brief Implementation for 'strndup' that copies a buffer [s] of at most [n] bytes
/// @param buffer Buffer to copy from
/// @param n Number of bytes to copy
/// @return String containing the copied buffer, returned buffers have to be freed manually
char* strndup(const char* buffer, size_t n);

#endif /* Utils_h */