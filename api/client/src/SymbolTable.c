#include "surf/HashTable.h"
#include "surf/Algorithm.h"
#include "surf/Define.h"

#include <stdlib.h>
#include <string.h>

/// @brief Represents a 'null' or deleted symbol in the table
static surf_HashTableItem HashTableDeletedItem = { NULL, NULL };

static surf_HashTableItem* surf_HashTableCreateItem(const char* id, void* fun)
{
    surf_HashTableItem* symbol = malloc(sizeof(surf_HashTableItem));

    if (symbol != NULL)
    {
		symbol->Identifier = STRDUP(id);
		symbol->Value = fun;
    }

    return symbol;
}

static void surf_HashTableDeleteValue(surf_HashTableItem* symbol)
{
    if (symbol == NULL)
        return;

    free(symbol->Identifier);
    symbol->Identifier = NULL;

    free(symbol);
    symbol = NULL;
}

static surf_HashTable* surf_HashTableCreateWithSize(const unsigned int size)
{
    surf_HashTable* table = malloc(sizeof(surf_HashTable));

    if (table != NULL)
    {
		table->Size = size;
		table->Count = 0;
		table->Items = calloc((size_t) table->Size, sizeof(surf_HashTableItem*));
    }

    return table;
}

surf_HashTable* surf_HashTableCreate()
{
    return surf_HashTableCreateWithSize(SURF_HASH_TABLE_STARTING_SIZE);
}

void surf_HashTableDestroy(surf_HashTable* table)
{
    for (size_t i = 0; i < table->Size; ++i) 
    {
        surf_HashTableItem* symbol = table->Items[i];
        if (symbol != &HashTableDeletedItem)
            surf_HashTableDeleteValue(symbol);
    }

    free(table);
    table = NULL;
}

static void surf_SymbolTableRehash(surf_HashTable* table) 
{
    // Copy data into fresh hash table with size
    surf_HashTable* newSymbolTable = surf_HashTableCreateWithSize(table->Size * 2);
    for (size_t i = 0; i < table->Size; ++i) 
    {
        surf_HashTableItem* item = table->Items[i];
        if (item != NULL && item != &HashTableDeletedItem) 
            surf_HashTableInsert(newSymbolTable, item->Identifier, item->Value);
    }

    table->Count = newSymbolTable->Count;

    const unsigned int tmpSize = table->Size;
    table->Size = newSymbolTable->Size;
    newSymbolTable->Size = tmpSize;

    surf_HashTableItem** tmpSymbols = table->Items;
    table->Items = newSymbolTable->Items;
    newSymbolTable->Items = tmpSymbols;

    surf_HashTableDestroy(newSymbolTable);
}

unsigned int surf_HashTableGetHash(const char* id, unsigned int size)
{
    return ElfHash(id, (unsigned int) strlen(id)) % size;
}

unsigned int surf_HashTableGetHashResolution(unsigned int size, unsigned int hash, unsigned int seed)
{
    return (hash + QuadraticResolution(seed)) % size;
}

void surf_HashTableInsert(surf_HashTable* table, const char* id, void* fun)
{
    // Check if table is above 70% capacity, double it if it needs more space
    const unsigned int loadFactor = table->Count * 100 / table->Size;
    if (loadFactor > 70)
        surf_SymbolTableRehash(table);

    surf_HashTableItem* newSymbol = surf_HashTableCreateItem(id, fun);
    unsigned int index = surf_HashTableGetHash(id, table->Size);
    unsigned int initial = index;

    unsigned int probeCount = 0;
    surf_HashTableItem* targetSymbol = table->Items[index];
    while (targetSymbol != NULL)
    {
        if (targetSymbol != &HashTableDeletedItem)
        {
            // Check for updates on an item already in the table
            if (strcmp(targetSymbol->Identifier, id) == 0)
            {
                surf_HashTableDeleteValue(targetSymbol);
                table->Items[index] = newSymbol;
                return;
            }
        }

        ++probeCount;
        index = surf_HashTableGetHashResolution(table->Size, index, probeCount);

        targetSymbol = table->Items[index];

        // Rehash if same item is probed
        if (index == initial)
            surf_SymbolTableRehash(table);
    }

    table->Items[index] = newSymbol;
    table->Count++;
}

void* surf_HashTableLookup(surf_HashTable* table, const char* id)
{
    unsigned int index = surf_HashTableGetHash(id, table->Size);
    unsigned int initial = index;
    surf_HashTableItem* item = table->Items[index];

    unsigned int probeCount = 0;
    while (item != NULL && item != &HashTableDeletedItem) 
    {
        if (strcmp(item->Identifier, id) == 0) 
            return item->Value;

        ++probeCount;
        index = surf_HashTableGetHashResolution(table->Size, index, probeCount);

        item = table->Items[index];

        if (index == initial)
            return NULL;
    } 

    return NULL;
}

void surf_HashTableRemove(surf_HashTable* table, const char* id)
{
    unsigned int index = surf_HashTableGetHash(id, table->Size);
    unsigned int initial = index;
    surf_HashTableItem* item = table->Items[index];

    unsigned int probeCount = 0;
    while (item != NULL) 
    {
        if (item != &HashTableDeletedItem) 
        {
            if (strcmp(item->Identifier, id) == 0) 
            {
                surf_HashTableDeleteValue(item);
                table->Items[index] = &HashTableDeletedItem;
                return;
            }
        }

        ++probeCount;
        index = surf_HashTableGetHashResolution(table->Size, initial, probeCount);

        item = table->Items[index];

        if (initial == index)
            return;
    } 

    table->Count--;
}