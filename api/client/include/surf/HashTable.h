#ifndef HashTable_h
#define HashTable_h

#define SURF_HASH_TABLE_STARTING_SIZE 23

/// @brief Represents a C function pointer stored 
typedef void (*surf_fun_t)(void*);

/// @brief Items in a hash table require an identifier and a generic value
typedef struct surf_HashTableItem
{
    char* Identifier;
    void* Value;
} surf_HashTableItem;

/// @brief Table containing a list of generic hash table items
typedef struct surf_HashTable
{
    unsigned int Size;
    unsigned int Count;
    surf_HashTableItem** Items;
} surf_HashTable;

/// @brief Allocates a hash table with entries based on SURF_HASH_TABLE_STARTING_SIZE
/// @return Pointer to the allocated hash table, must eventually be deleted with 'surf_HashTableDestroy'
surf_HashTable* surf_HashTableCreate();

/// @brief Frees memory associated with each item in a hash table
/// @param table The hash table to free in memory
void surf_HashTableDestroy(surf_HashTable* table);

/// @brief Calculates the hash value for a string identifier in the hash table
/// @param id String identifier for the entry being hashed
/// @param size Current size of the hash table, used for bounding the hash value
/// @return Index calculated in the hash table for the identifier 
unsigned int surf_HashTableGetHash(const char* id, unsigned int size);

/// @brief Calculates the resolution value for collisions based on a quadratic resolution function
/// @param size Current size of the hash table, used for bounding the resolution hash value
/// @param hash Last hash value used to hash the identifier in the table
/// @param seed Seed for the resolution function
/// @return Resolution index for the item in the hash table
unsigned int surf_HashTableGetHashResolution(unsigned int size, unsigned int hash, unsigned int seed);

/// @brief Inserts a symbol into the table with a provided string identifier and a function pointer
/// @param table Table to insert the value into
/// @param id String identifier to lookup/retrieve the item in the table 
/// @param value Generic pointer for the value
void surf_HashTableInsert(surf_HashTable* table, const char* id, void* value);

/// @brief Looks up a value associated with an identifier in the hash table
/// @param table Table to perform the lookup on
/// @param id String identifier to lookup in the symbol table
/// @return Value pointer associated with the passed identifier, NULL if not found
void* surf_HashTableLookup(surf_HashTable* table, const char* id);

/// @brief Removes an entry associated with a provided identifier in the hash table
/// @param table Table to perform the removal on
/// @param id Identifier associated with the item being removed from the hash table
void surf_HashTableRemove(surf_HashTable* table, const char* id);

#endif /* HashTable_h */