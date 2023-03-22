#ifndef Utils_h
#define Utils_h

#include <sys/types.h>

/// @brief Retrieves the last modified timestamp for a passed filepath
/// @param filepath Path of the file to get the last modified timestamp for
/// @return Last modified timestamp
time_t GetFileLastModifiedTime(const char* filepath);

/// @brief Checks if the passed filepath has been modified since a particular timestamp
/// @param filepath Path of the file to check
/// @param lastModifiedTime Last modified time of the file
/// @return True if the file has been modified since the passed timestamp, false otherwise
int IsFileModified(const char* filepath, time_t lastModifiedTime);

/// @brief Splits a string with a provided delimeter
/// @param str String to split
/// @param delimeter Delimeter to split the string against
/// @return Array containing the result of splitting the string, must be freed after use
/// @reference https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
char** StringSplit(char* str, const char delimeter);

/// @brief Converts a string to a lowercase representation of itself
/// @param str String to convert to lowercase
void StringToLower(char** str);

/// @brief Checks if a passed string is empty
/// @param str String to evaluate
/// @return True if the string is empty, false if it is not
int IsStringEmpty(char* str);

/// @brief Checks if a passed string is a valid integer
/// @param str String to evaluate
/// @return True if the string is an integer, false if it is not
int IsStringInt(char* str);

/// @brief Checks if a passed string is a valid float
/// @param str String to evaluate
/// @return True if the string is a float, false if it is not
int IsStringFloat(char* str);

#ifdef WIN32
/// @brief Variadic function to allocate memory for a buffer based on it's variadic arguments, the buffer `pStr` should be freed
/// @param pStr Buffer to allocate for the string
/// @param fmt C-style formatting string
/// @param Variadic arguments to place into the format string
/// @return Number of bytes in the string that was allocated, -1 if none were
/// @reference https://stackoverflow.com/questions/40159892/using-asprintf-on-windows
int vasprintf(char** pStr, const char* fmt, ...);
#endif

#endif /* Utils_h */