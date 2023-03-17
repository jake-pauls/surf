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

        // Keyword #1 should be the reflection keyword for it to be executed
        if (strcmp(split[0], "ref") == 0) 
		    surf_InternalExecuteReflectionCallback(buffer);

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

    surf_SymbolTableInsert(s_SymbolTable, id, func);
}

void surf_InternalExecuteReflectionCallback(const char* buffer)
{
    // Prevent retrieving reflected functions if they don't exist
    if (s_SymbolTable == NULL)
    {
        SURF_API_CLIENT_LOG("Attempted to reflect a function in surf, but no functions have been registered in the API");
        return;
    }

    char* dupBuffer = STRDUP(buffer);
	const char space = ' ';
	char** args = StringSplit(dupBuffer, space);

    // Function should be the first string after 'ref'
    char* identifier = args[1];
    surf_fun_t callback = surf_SymbolTableLookup(s_SymbolTable, identifier);

    // Retrieve the length of the args to check for argument pack
    int splitLen = -1;
    while (args[++splitLen] != NULL);
    
    // Reflected function has the signature (void)(void), and can be instantly called
    if (splitLen <= 2)
    {
		callback();
        free(args);
        free(dupBuffer);
        return;
    } 

    // Setup array of generic arguments
	int vArgsIndex = 0;
	void** vArgs = (void**) malloc((splitLen - 2) * sizeof(void*));

	for (size_t i = 2; i < splitLen; ++i)
	{
		const char typeDelimeter = ':';
		char** splitArg = StringSplit(args[i], typeDelimeter);

		char* argValue = splitArg[0];
		char* argType = splitArg[1];

        // Retrieve a reference to the value and match it against its surf type
		// Note: Currently only marshals 'int', 'flt', and 'str' types

		if (strcmp(argType, "int") == 0)
		{
			int intArg = atoi(argValue);
			vArgs[vArgsIndex] = &intArg;
		}
		else if (strcmp(argType, "flt") == 0)
		{
			float fltArg = (float) atof(argValue);
			vArgs[vArgsIndex] = &fltArg;
		}
		else if (strcmp(argType, "str") == 0)
		{
			vArgs[vArgsIndex] = &argValue;
		}

		vArgsIndex++;

		free(splitArg);
	}

	// Function has some args, client function is responsible for having a 'void**' to catch them
	// Casting each arg to its appropriate type and passing it requires metaprogramming or lots more work
	// Could be a TODO, but seems unnecessary to prove the concept of surf
	callback(vArgs);

	free(vArgs);
    free(args);
    free(dupBuffer);
}

void surf_InternalInterpDestroy()
{
    if (s_SymbolTable != NULL)
        surf_SymbolTableDestroy(s_SymbolTable);
}

