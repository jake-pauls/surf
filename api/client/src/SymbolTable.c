#include "surf/SymbolTable.h"
#include "surf/Algorithm.h"
#include "surf/Define.h"

#include <stdlib.h>
#include <string.h>

/// @brief Represents a 'null' or deleted symbol in the table
static surf_Symbol SymbolTableDeletedItem = { NULL, NULL };

static surf_Symbol* surf_SymbolTableCreateSymbol(const char* id, surf_fun_t fun)
{
    surf_Symbol* symbol = malloc(sizeof(surf_Symbol));

    if (symbol != NULL)
    {
		symbol->Identifier = STRDUP(id);
		symbol->Function = fun;
    }

    return symbol;
}

static void surf_SymbolTableDeleteSymbol(surf_Symbol* symbol)
{
    free(symbol->Identifier);
    symbol->Identifier = NULL;

    free(symbol);
    symbol = NULL;
}

static surf_SymbolTable* surf_SymbolTableCreateWithSize(const unsigned int size)
{
    surf_SymbolTable* table = malloc(sizeof(surf_SymbolTable));

    if (table != NULL)
    {
		table->Size = size;
		table->Count = 0;
		table->Items = calloc((size_t) table->Size, sizeof(surf_Symbol*));
    }

    return table;
}

surf_SymbolTable* surf_SymbolTableCreate()
{
    return surf_SymbolTableCreateWithSize(SURF_SYMBOL_TABLE_STARTING_SIZE);
}

void surf_SymbolTableDestroy(surf_SymbolTable* table)
{
    for (size_t i = 0; i < table->Size; ++i) 
    {
        surf_Symbol* symbol = table->Items[i];
        if (symbol != NULL)
            surf_SymbolTableDeleteSymbol(symbol);
    }

    free(table);
    table = NULL;
}

static void surf_SymbolTableRehash(surf_SymbolTable* table) 
{
    // Copy data into fresh hash table with size
    surf_SymbolTable* newSymbolTable = surf_SymbolTableCreateWithSize(table->Size * 2);
    for (size_t i = 0; i < table->Size; ++i) 
    {
        surf_Symbol* item = table->Items[i];
        if (item != NULL && item != &SymbolTableDeletedItem) 
            surf_SymbolTableInsert(newSymbolTable, item->Identifier, item->Function);
    }

    table->Count = newSymbolTable->Count;

    const unsigned int tmpSize = table->Size;
    table->Size = newSymbolTable->Size;
    newSymbolTable->Size = tmpSize;

    surf_Symbol** tmpSymbols = table->Items;
    table->Items = newSymbolTable->Items;
    newSymbolTable->Items = tmpSymbols;

    surf_SymbolTableDestroy(newSymbolTable);
}

unsigned int surf_SymbolTableGetHash(const char* id, unsigned int size)
{
    return ElfHash(id, (unsigned int) strlen(id)) % size;
}

unsigned int surf_SymbolTableGetHashResolution(unsigned int size, unsigned int hash, unsigned int seed)
{
    return (hash + QuadraticResolution(seed)) % size;
}

void surf_SymbolTableInsert(surf_SymbolTable* table, const char* id, surf_fun_t fun)
{
    // Check if table is above 70% capacity, double it if it needs more space
    const unsigned int loadFactor = table->Count * 100 / table->Size;
    if (loadFactor > 70)
        surf_SymbolTableRehash(table);

    surf_Symbol* newSymbol = surf_SymbolTableCreateSymbol(id, fun);
    unsigned int index = surf_SymbolTableGetHash(id, table->Size);
    unsigned int initial = index;

    unsigned int probeCount = 0;
    surf_Symbol* targetSymbol = table->Items[index];
    while (targetSymbol != NULL)
    {
        if (targetSymbol != &SymbolTableDeletedItem)
        {
            // Check for updates on an item already in the table
            if (strcmp(targetSymbol->Identifier, id) == 0)
            {
                surf_SymbolTableDeleteSymbol(targetSymbol);
                table->Items[index] = newSymbol;
                return;
            }
        }

        ++probeCount;
        index = surf_SymbolTableGetHashResolution(table->Size, index, probeCount);

        targetSymbol = table->Items[index];

        // Rehash if same item is probed
        if (index == initial)
            surf_SymbolTableRehash(table);
    }

    table->Items[index] = newSymbol;
    table->Count++;
}

surf_fun_t surf_SymbolTableLookup(surf_SymbolTable* table, const char* id)
{
    unsigned int index = surf_SymbolTableGetHash(id, table->Size);
    unsigned int initial = index;
    surf_Symbol* item = table->Items[index];

    unsigned int probeCount = 0;
    while (item != NULL && item != &SymbolTableDeletedItem) 
    {
        if (strcmp(item->Identifier, id) == 0) 
            return item->Function;

        ++probeCount;
        index = surf_SymbolTableGetHashResolution(table->Size, index, probeCount);

        item = table->Items[index];

        if (index == initial)
            return NULL;
    } 

    return NULL;
}

void surf_SymbolTableRemove(surf_SymbolTable* table, const char* id)
{
    unsigned int index = surf_SymbolTableGetHash(id, table->Size);
    unsigned int initial = index;
    surf_Symbol* item = table->Items[index];

    int probeCount = 0;
    while (item != NULL) 
    {
        if (item != &SymbolTableDeletedItem) 
        {
            if (strcmp(item->Identifier, id) == 0) 
            {
                surf_SymbolTableDeleteSymbol(item);
                table->Items[index] = &SymbolTableDeletedItem;
            }
        }

        ++probeCount;
        index = surf_SymbolTableGetHashResolution(table->Size, initial, probeCount);

        item = table->Items[index];

        if (initial == index)
            return;
    } 

    table->Count--;
}