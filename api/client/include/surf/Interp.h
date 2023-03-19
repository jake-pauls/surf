#ifndef Interp_h
#define Interp_h

#include "surf/Vec.h"
#include "surf/SymbolTable.h"

/// @brief Directly submits a line of code to the interpreter through the API, all strings returned must be destroyed with 'surf_InterpFreeString'
/// @param line String buffer containing the line of code to submit to the interpreter
/// @return String buffer containing the interpreted result
/// @note Requires a valid cfg.surf file loaded with 'surf_CfgLoad' prior to use
char* surf_InterpLine(const char* line);
 
/// @brief Reads and submits requests to interpret a surf file line-by-line 
/// @param filepath Absolute path (or path relative to this project) to the surf script to interpret
/// @return True if the file was succesfully interpreted, false if it was not
/// @note Requires a valid cfg.surf file loaded with 'surf_CfgLoad' prior to use
int surf_InterpFile(const char* filepath);

/// @brief Wrapper to destroy results from 'surf_InterpLine'
/// @param line String allocated as a result of 'surf_InterpLine'
void surf_InterpFreeString(char* line);

/// @brief Retrieves an integer variable from the current surf environment
/// @param name Name of the variable to retrieve
/// @param out Address to return the resultant integer into
/// @return True if retrieving the integer succeeded, false if it did not (ie: retrieved variable was actually of type 'str')
int surf_InterpGetInt(const char* name, int* out);

/// @brief Retrieves a float variable from the current surf environment
/// @param name Name of the variable to retrieve
/// @param out Address to retrun the resultant float into
/// @return True if retrieving the float succeeded, false if it did not (ie: retrieved variable was actually of type 'str')
int surf_InterpGetFlt(const char* name, float* out);

/// @brief Retrieves a string variable from the current surf environment
/// @param name Name of the variable to retrieve
/// @param out Address to return the resultant string into, strings are allocated and must be freed using 'surf_InterpFreeString'
/// @return True if retrieving the string succeeded, false if it did not (ie: string is empty)
int surf_InterpGetStr(const char* name, char** out);

/// @brief Retrieves a v2 variable from the current surf environment
/// @param name Name of the variable to retrieve
/// @param out Address to return the resultant vector components into
/// @return True if retrieving the vector and it's components succeeded, false if it did not
int surf_InterpGetV2(const char* name, surf_V2* out);

/// @brief Retrieves a v3 variable from the current surf environment
/// @param name Name of the variable to retrieve
/// @param out Address to return the resultant vector components into
/// @return True if retrieving the vector and it's components succeeded, false if it did not
int surf_InterpGetV3(const char* name, surf_V3* out);

/// @brief Retrieves a v4 variable from the current surf environment
/// @param name Name of the variable to retrieve
/// @param out Address to return the resultant vector components into
/// @return True if retrieving the vector and it's components succeeded, false if it did not
int surf_InterpGetV4(const char* name, surf_V4* out);

/// @brief Creates a let binding for an int variable in the current surf environment
/// @param name Name of the variable to bind
/// @param i Integer value to bind to the variable
void surf_InterpBindInt(const char* name, int i);

/// @brief Creates a let binding for a flt variable in the current surf environment
/// @param name Name of the variable to bind
/// @param f Float value to bind to the variable
void surf_InterpBindFlt(const char* name, float f);

/// @brief Creates a let binding for a str variable in the current surf environment
/// @param name Name of the variable to bind
/// @param str String value to bind to the variable
void surf_InterpBindStr(const char* name, const char* str);

/// @brief Creates a let binding for a v2 variable in the current surf environment
/// @param name Name of the variable to bind
/// @param f1 Vector x-component
/// @param f2 Vector y-component
void surf_InterpBindV2(const char* name, float f1, float f2);

/// @brief Creates a let binding for a v3 variable in the current surf environment
/// @param name Name of the variable to bind
/// @param f1 Vector x-component
/// @param f2 Vector y-component
/// @param f3 Vector z-component
void surf_InterpBindV3(const char* name, float f1, float f2, float f3);

/// @brief Creates a let binding for a v4 variable in the current surf environment
/// @param name Name of the variable to bind
/// @param f1 Vector x-component
/// @param f2 Vector y-component
/// @param f3 Vector z-component
/// @param f4 Vector w-component
void surf_InterpBindV4(const char* name, float f1, float f2, float f3, float f4);

/// @brief Registers a symbol in the surf API, once the symbol is reflected in a surf script - the callback 'func' will be executed
/// @param id The identifier for the symbol, this must match the name of the symbol being reflected in surf
/// @param fun Callback function to be called when 'ref id()' is executed in a surf script
void surf_InterpRegisterSymbol(const char* id, surf_fun_t fun);

/// @brief Unregisters a symbol in the surf API 
/// @param id The identifier for the symbol being unregistered
void surf_InterpUnregisterSymbol(const char* id);

/// @brief INTERNAL: Parses a returned buffer from the interpreter for a reflected function and executes it's associated callback in the symbol table
/// @param buffer String buffer containing the reflected function in surf
/// @note Only handles/reflects parameters with 'int', 'flt', or 'str' types
void surf_InternalExecuteReflectionCallback(const char* buffer);

/// @brief Destroys the APIs symbol table if it has symbols registered
void surf_InternalInterpDestroy();

#endif /* Interp_h */