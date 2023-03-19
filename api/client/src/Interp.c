#include "surf/Interp.h"
#include "surf/Bridge.h"
#include "surf/Define.h"
#include "surf/Utils.h"
#include "surf/Cfg.h"
#include "surf/Vec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static surf_SymbolTable* s_SymbolTable = NULL;

char* surf_InterpLine(const char* line)
{
    // Bail out of all interpreter requests if no cfg.surf file is present 
    // TODO: Find a way to do this that is less ironic?
    if (!surf_CfgIsValidLoaded() && !strstr(line, "cfg"))
        return NULL;

    int _ = surf_InternalSendSocket(line, strlen(line), 0);

    // Create string buffer
    char buffer[SURF_MAX_BUFFER_SIZE];
    size_t bufferLen = SURF_MAX_BUFFER_SIZE;

    // Receive from the interpreter
    _ = surf_InternalReceiveSocket(buffer, bufferLen, 0);

    // Check if buffer is a function reflected in the API
    // Have to cross-check with the original str of surf code to make sure interpreter isn't returning string
    // This can be much much more clean if the server also returned a command type
    // Due to the nature of this being over a network - namely with a Mingw server, the only other issue is passing structs over sockets is prone to mangling 

    // Preliminary check: check if the str has 'ref' at all 
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

int surf_InterpFile(const char* filepath)
{
    FILE* handle = fopen(filepath, "r");
    if (handle == NULL)
        return SURF_FALSE;

    char str[SURF_MAX_BUFFER_SIZE];
    while (fgets(str, SURF_MAX_BUFFER_SIZE, handle))
    {
        // TODO: This is kind of unsafe...
        surf_InterpLine(str);
    }

    fclose(handle);
    return SURF_TRUE;
}

void surf_InterpFreeString(char* str)
{
    if (str == NULL)
        return; 

    free(str);
    str = NULL;
}

int surf_InterpGetInt(const char* name, int* out) 
{
    const char* fmt = "put(%s);";

    char* buffer;
    int _ = ASPRINTF(&buffer, fmt, name);

    char* ret = surf_InterpLine(buffer);
    if (!IsStringInt(ret) || IsStringEmpty(ret))
        return SURF_FALSE;

    *(out) = atoi(ret);

    free(ret);
	free(buffer);

    return SURF_TRUE;
}

int surf_InterpGetFlt(const char* name, float* out)
{
    const char* fmt = "put(%s);";

    char* buffer;
    int _ = ASPRINTF(&buffer, fmt, name);

    char* ret = surf_InterpLine(buffer);
    if (!IsStringFloat(ret) || IsStringEmpty(ret))
        return SURF_FALSE;

    *(out) = (float) atof(ret);

    free(ret);
    free(buffer);

    return SURF_TRUE;
}

int surf_InterpGetStr(const char* name, char** out)
{
    const char* fmt = "put(%s);";

    char* buffer;
    int _ = ASPRINTF(&buffer, fmt, name);

    // TODO: Check internal types of the interpreter to ensure this is a string
    char* ret = surf_InterpLine(buffer);
    if (IsStringEmpty(ret))
        return SURF_FALSE;

    // Allocate string for the user, doing this for consistency with other 'surf_InterpGet*' functions
    // Clients must use 'surf_InterpFreeString' to release this string
    _ = ASPRINTF(out, "%s", ret);

    free(ret);
    free(buffer);

    return SURF_TRUE;
}

int surf_InterpGetV2(const char* name, surf_V2* out)
{
    const char* fmt = "put(%s);";

    char* buffer;
    int _ = ASPRINTF(&buffer, fmt, name);

    // TODO: Check internal types of the interpreter to ensure this is a string
    char* ret = surf_InterpLine(buffer);
    if (IsStringEmpty(ret))
        return SURF_FALSE;

    surf_VecV2FromStr(ret, out);

    free(ret);
    free(buffer);

    return SURF_TRUE;
}

int surf_InterpGetV3(const char* name, surf_V3* out)
{
    const char* fmt = "put(%s);";

    char* buffer;
    int _ = ASPRINTF(&buffer, fmt, name);

    // TODO: Check internal types of the interpreter to ensure this is a string
    char* ret = surf_InterpLine(buffer);
    if (IsStringEmpty(ret))
        return SURF_FALSE;

    surf_VecV3FromStr(ret, out);

    free(ret);
    free(buffer);

    return SURF_TRUE;
}

int surf_InterpGetV4(const char* name, surf_V4* out)
{
    const char* fmt = "put(%s);";

    char* buffer;
    int _ = ASPRINTF(&buffer, fmt, name);

    // TODO: Check internal types of the interpreter to ensure this is a string
    char* ret = surf_InterpLine(buffer);
    if (IsStringEmpty(ret))
        return SURF_FALSE;

    surf_VecV4FromStr(ret, out);

    free(ret);
    free(buffer);

    return SURF_TRUE;
}

void surf_InterpBindInt(const char* name, int i)
{
    const char* fmt = "let %s: int = %d;";

    char* buffer;
    int _ = ASPRINTF(&buffer, fmt, name, i);

    // Interpret and discard the result
    char* ret = surf_InterpLine(buffer);
    free(ret);

    free(buffer);
}

void surf_InterpBindFlt(const char* name, float f)
{
    const char* fmt = "let %s: flt = %f;";

    char* buffer;
    int _ = ASPRINTF(&buffer, fmt, name, f);

    // Interpret and discard the result
    char* ret = surf_InterpLine(buffer);
    free(ret);

    free(buffer);
}

void surf_InterpBindStr(const char* name, const char* str)
{
    const char* fmt = "let %s: str = \"%s\";";

    char* buffer;
    int _ = ASPRINTF(&buffer, fmt, name, str);

    // Interpret and discard the result
    char* ret = surf_InterpLine(buffer);
    free(ret);

    free(buffer);
}

void surf_InterpBindV2(const char* name, float f1, float f2)
{
    const char* fmt = "let %s: v2 = (%f,%f);";

    char* buffer;
    int _ = ASPRINTF(&buffer, fmt, name, f1, f2);

    // Interpret and discard the result
    char* ret = surf_InterpLine(buffer);
    free(ret);

    free(buffer);
}

void surf_InterpBindV3(const char* name, float f1, float f2, float f3)
{
    const char* fmt = "let %s: v3 = (%f,%f,%f);";

    char* buffer;
    int _ = ASPRINTF(&buffer, fmt, name, f1, f2, f3);

    // Interpret and discard the result
    char* ret = surf_InterpLine(buffer);
    free(ret);

    free(buffer);
}

void surf_InterpBindV4(const char* name, float f1, float f2, float f3, float f4)
{
    const char* fmt = "let %s: v4 = (%f,%f,%f,%f);";

    char* buffer;
    int _ = ASPRINTF(&buffer, fmt, name, f1, f2, f3, f4);

    // Interpret and discard the result
    char* ret = surf_InterpLine(buffer);
    free(ret);

    free(buffer);
}

void surf_InterpRegisterSymbol(const char* id, surf_fun_t fun)
{
    if (s_SymbolTable == NULL)
        s_SymbolTable = surf_SymbolTableCreate();

    surf_SymbolTableInsert(s_SymbolTable, id, fun);
}

void surf_InterpUnregisterSymbol(const char* id)
{
    if (s_SymbolTable == NULL)
        return;

    surf_SymbolTableRemove(s_SymbolTable, id);
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

    // Exit out if function isn't found
    surf_fun_t callback = surf_SymbolTableLookup(s_SymbolTable, identifier);
    if (callback == NULL)
        return;

    // Retrieve the length of the args to check for argument pack
    int splitLen = -1;
    while (args[++splitLen] != NULL);
    
    // Reflected function has the signature (void)(void), and can be instantly called
    if (splitLen <= 2)
    {
		callback(NULL);
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
        else if (strcmp(argType, "v2") == 0)
        {
            surf_V2 v2 = { 0.0f, 0.0f };
            surf_VecV2FromStr(argValue, &v2);
            vArgs[vArgsIndex] = &v2;
        }
        else if (strcmp(argType, "v3") == 0)
        {
            surf_V3 v3 = { 0.0f, 0.0f, 0.0f };
            surf_VecV3FromStr(argValue, &v3);
            vArgs[vArgsIndex] = &v3;
        }
        else if (strcmp(argType, "v4") == 0)
        {
            surf_V4 v4 = { 0.0f, 0.0f, 0.0f, 0.0f };
            surf_VecV4FromStr(argValue, &v4);
            vArgs[vArgsIndex] = &v4;
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
    if (s_SymbolTable)
		surf_SymbolTableDestroy(s_SymbolTable);
}

