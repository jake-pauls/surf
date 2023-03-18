#ifndef Interp_h
#define Interp_h

#include "surf/SymbolTable.h"

/// @brief Directly submits a line of code to the interpreter through the API, all strings returned must be destroyed with 'surf_InterpDestroyLine'
/// @param line String buffer containing the line of code to submit to the interpreter
/// @return String buffer containing the interpreted result
char* surf_InterpLine(const char* line);

/// @brief Wrapper to destroy results from 'surf_InterpLine'
/// @param line String allocated as a result of 'surf_InterpLine'
void surf_InterpDestroyLine(char* line);

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