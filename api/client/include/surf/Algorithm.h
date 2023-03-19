#ifndef Algorithm_h
#define Algorithm_h

/// @brief Implementation of the ElfHash algorithm
/// @param str String buffer containing data to represent as a hash
/// @param strLen Length of the string buffer
/// @return Computed ElfHash of the string buffer
/// @reference https://www.programmingalgorithms.com/algorithm/elf-hash/c/
unsigned int ElfHash(const char* str, unsigned int strLen);

/// @brief Quadratic resolution function for a hash table
unsigned int QuadraticResolution(int i);

#endif /* Algorithm_h */