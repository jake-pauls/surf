#ifndef SymbolTable_h
#define SymbolTable_h

#define SURF_SYMBOL_TABLE_STARTING_SIZE 23

/// @brief Represents a C function pointer stored in the symbol table
typedef void (*surf_fun_t)(void*);

/// @brief Symbols registered in the surf API require an identifier and a valid function pointer
typedef struct surf_Symbol
{
    char* Identifier;
    surf_fun_t Function;
} surf_Symbol;

/// @brief Table containing a list of registered symbols
typedef struct surf_SymbolTable
{
    unsigned int Size;
    unsigned int Count;
    surf_Symbol** Items;
} surf_SymbolTable;

/// @brief Allocates a symbol table with entries based on SURF_SYMBOL_TABLE_STARTING_SIZE
/// @return Pointer to the allocated symbol table, must eventually be deleted with 'surf_SymbolTableDelete'
surf_SymbolTable* surf_SymbolTableCreate();

/// @brief Frees memory associated with each symbol in a symbol table
/// @param table The symbol table to free in memory
void surf_SymbolTableDestroy(surf_SymbolTable* table);

/// @brief Calculates the hash value for a string identifier in the symbol table
/// @param id String identifier for the entry being hashed
/// @param size Current size of the hash table, used for bounding the hash value
/// @return Index calculated in the hash table for the identifier 
unsigned int surf_SymbolTableGetHash(const char* id, unsigned int size);

/// @brief Calculates the resolution value for collisions based on a quadratic resolution function
/// @param size Current size of the hash table, used for bounding the resolution hash value
/// @param hash Last hash value used to hash the identifier in the table
/// @param seed Seed for the resolution function
/// @return Resolution index for the item in the hash table
unsigned int surf_SymbolTableGetHashResolution(unsigned int size, unsigned int hash, unsigned int seed);

/// @brief Inserts a symbol into the table with a provided string identifier and a function pointer
/// @param table Table to insert the symbol into
/// @param id String identifier to lookup/retrieve the item in the table 
/// @param fun Function pointer for the symbol's behaviour on the client
void surf_SymbolTableInsert(surf_SymbolTable* table, const char* id, surf_fun_t fun);

/// @brief Looks up a function pointer associated with an identifier in the symbol table
/// @param table Table to perform the lookup on
/// @param id String identifier to lookup in the symbol table
/// @return Function pointer associated with the passed identifier, NULL if not found
surf_fun_t surf_SymbolTableLookup(surf_SymbolTable* table, const char* id);

/// @brief Removes an entry associated with a provided identifier in the symbol table
/// @param table Table to perform the removal on
/// @param id Identifier associated with the item being removed from the symbol table
void surf_SymbolTableRemove(surf_SymbolTable* table, const char* id);

#endif /* SymbolTable_h */