#include "surf/Interp.h"
#include "surf/Bridge.h"
#include "surf/Define.h"
#include "surf/Utils.h"

#include <stdlib.h>
#include <string.h>

static surf_SymbolTable* s_SymbolTable = NULL;

char* surf_InterpLine(const char* line)
{
    int _ = surf_InternalSendSocket(line, strlen(line), 0);

    // Create string buffer
    char buffer[SURF_MAX_BUFFER_SIZE];
    size_t bufferLen = SURF_MAX_BUFFER_SIZE;

    // Receive from the interpreter
    _ = surf_InternalReceiveSocket(buffer, bufferLen, 0);

    // Check if buffer is a function reflected in the API
    // Have to cross-check with the original line of surf code to make sure interpreter isn't returning string
    // This can be much much more clean if the server also returned a command type
    // Due to the nature of this being over a network - namely with a Mingw server, the only other issue is passing structs over sockets is prone to mangling 

    // Preliminary check: check if the line has 'ref' at all 
    char* dupLine = STRDUP(line);
    if (strstr(line, "ref"))
    {
        const char space = ' ';
        char** split = StringSplit(dupLine, space);

        // If keyword #1 of the executed line was a reflection keyword, search and execute the registered function 
        if (strcmp(split[0], "ref") == 0) 
        {
            SURF_API_CLIENT_LOG("ref was executed!");
		    surf_InternalParseReflection(buffer);
        }

        free(split);
    }
    free(dupLine);

    return STRDUP(buffer);
}

void surf_InterpDestroyLine(char* line)
{
    if (line == NULL)
        return; 

    free(line);
    line = NULL;
}

void surf_InterpRegisterSymbol(const char* id, surf_fun_t func)
{
    if (s_SymbolTable == NULL)
        s_SymbolTable = surf_SymbolTableCreate();

    SURF_API_CLIENT_LOG("Registered symbol with id: %s", id);

    surf_SymbolTableInsert(s_SymbolTable, id, func);
}

void surf_InternalParseReflection(const char* buffer)
{
    // Prevent retrieving reflected functions if they don't exist
    if (s_SymbolTable == NULL)
    {
        SURF_API_CLIENT_LOG("Attempted to reflect a function in surf, but no functions have been registered in the API");
        return;
    }

    char* dupBuffer = STRDUP(buffer);
	const char space = ' ';
	char** split = StringSplit(dupBuffer, space);

    // Function should be the first string after 'ref'
    char* id = split[1];
    SURF_API_CLIENT_LOG("Looking up reflected function %s", id);
    surf_fun_t callback = surf_SymbolTableLookup(s_SymbolTable, id);
    
    // Execute the callback with the internal argument
    callback(SURF_FUN_ARG_INTERNAL);

    free(split);
    free(dupBuffer);
}

void surf_InternalInterpDestroy()
{
    if (s_SymbolTable != NULL)
        surf_SymbolTableDestroy(s_SymbolTable);
}

