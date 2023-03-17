#ifndef Utils_h
#define Utils_h

/// @brief Splits a string with a provided delimeter
/// @param str String to split
/// @param delimeter Delimeter to split the string against
/// @return Array containing the result of splitting the string, must be freed after use
/// @reference https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
char** StringSplit(char* str, const char delimeter);

#endif /* Utils_h */